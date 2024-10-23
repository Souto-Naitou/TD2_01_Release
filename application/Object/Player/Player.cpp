#include "Player.h"

#include "Helper/DefaultSettings.h"
#include "Externals/ImGuiDebugManager/DebugManager.h"
#include "Object/Enemy/Enemy.h"
#include <type_traits>
#include "Collision/CollisionManager/CollisionManager.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


Player::Player()
{
    pCollisionManager_ = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("Player", std::bind(&Player::DebugWindow, this));
    pEnemyManager_ = EnemyManager::GetInstance();
    pDraw2D_ = Draw2D::GetInstance();
    pInput_ = Input::GetInstance();
    pRotateBoardManager_ = RotateBoardManager::GetInstance();
}

Player::~Player()
{
    DebugManager::GetInstance()->DeleteComponent("Player");
}

void Player::Initialize()
{
    startTime_ = std::chrono::system_clock::now();
    pEasingBoxResize_ = std::make_unique<Easing>("DecreaseSize");
    pEasingBoxTemp_ = std::make_unique<Easing>("IncreaseSize");

    position_.x = static_cast<float>(DefaultSettings::kGameScreenWidth / 2ui32 + DefaultSettings::kGameScenePosX);
    position_.y = static_cast<float>(DefaultSettings::kGameScreenHeight / 2ui32 + DefaultSettings::kGameScenePosY);
    radius_current_ = radius_default_;

    /// コライダー関連
    // 所有者を登録
    collider_.SetOwner(this);

    // コライダー識別子を登録
    collider_.SetColliderID("Player");

    // アトリビュートの生成・登録
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("Player"));

    // colliderにポインタを渡す
    collider_.SetOnCollision(std::bind(&Player::OnCollision, this, std::placeholders::_1));

    // 衝突用座標 (ラグ軽減用)
    collider_.SetPosition(position_);

    // ラグ軽減の有無
    collider_.SetEnableLighter(false);

    // colliderの登録
    pCollisionManager_->RegisterCollider(&collider_);


    /// 回転板の初期化
    pRotateBoard_ = new RotateBoard();
    pRotateBoard_->Initialize();
    pRotateBoard_->SetVertices(&vertices_);
    pRotateBoard_->SetParentPosition(position_);
}

void Player::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask(collider_.GetColliderID(), "Core", "NestWall"));
    pRotateBoard_->RunSetMask();
}

void Player::Update()
{
    if (pInput_->PushKey(DIK_SPACE))
    {
        isAttack_ = false;
        if (radius_current_ > radius_min_)
        {
            pEasingBoxTemp_->Reset();
            pEasingBoxResize_->Start();
            radius_current_ = (1.0f - pEasingBoxResize_->Update()) * radius_default_ + pEasingBoxResize_->Update() * radius_min_;
        }
        // 吸い込み力をEasingのCurrentTにする
        pEnemyManager_->SetStateSuction(pEasingBoxResize_->GetCurrentT());
    }
    else if (pInput_->ReleaseKey(DIK_SPACE))
    {
        /// スペースを離したら
        latestAttackMultiply_ = pEasingBoxResize_->GetCurrentT();
        radius_timeRelease_ = radius_current_;
        isAttack_ = true;
        pEnemyManager_->ChangeState(Enemy::State::Normal);
        pRotateBoardManager_->SetLeaveFlag(true);
    }
    else
    {
        if (pEasingBoxTemp_->GetIsEnd())
        {
            isAttack_ = false;
            pRotateBoardManager_->SetIsAttack(false);
        }
        pEasingBoxResize_->Reset();
        pEasingBoxTemp_->Start();
        radius_current_ = (1.0f - pEasingBoxTemp_->Update()) * radius_timeRelease_ + pEasingBoxTemp_->Update() * radius_default_;
    }

    if (vertices_.size() != resolution_)
    {
        vertices_.resize(resolution_);
    }

    float theta = 0;
    for (int i = 0; i < resolution_; i++)
    {
        Vector2 result = {};
        theta += 2.0f / resolution_ * 3.141592f;
        result.x = position_.x + std::cosf(theta) * radius_current_ - std::sinf(theta) * radius_current_;
        result.y = std::sinf(theta) * radius_current_ + position_.y + std::cosf(theta) * radius_current_;
        vertices_[i] = result;
    }

    collider_.SetRadius(static_cast<int>(radius_current_ * 1.5));
    collider_.SetVertices(&vertices_);
    pRotateBoard_->Update();
}

void Player::Draw()
{
    Vector4 white = { 1.0f, 1.0f, 1.0f, 1.0f };

    for (int i = 0; i < resolution_ - 1; i++)
    {
        pDraw2D_->DrawLine(vertices_[i], vertices_[i + 1], white);
    }

    pDraw2D_->DrawLine(vertices_[resolution_ - 1], vertices_[0], white);

    pRotateBoard_->Draw();
}

void Player::DebugWindow()
{
#ifdef _DEBUG

    ImGui::DragFloat2("座標", &position_.x, 1.0f);
    ImGui::InputInt("頂点数", reinterpret_cast<int*>(&resolution_));
    if (!resolution_) { resolution_ = 1; }

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("circle_", vertices_);
        ImGuiTemplate::VariableTableRow("isAttack_", isAttack_);
        ImGuiTemplate::VariableTableRow("radius_current_", radius_current_);
        ImGuiTemplate::VariableTableRow("radius_timeRelease_", radius_timeRelease_);
        ImGuiTemplate::VariableTableRow("radius_default_", radius_default_);
        ImGuiTemplate::VariableTableRow("radius_min_", radius_min_);
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable("Player", pFunc);

#endif // _DEBUG
}

void Player::OnCollision(const Collider* _other) {
    //_otherがEnemyかどうかを確認
    if (_other->GetColliderID() == "Enemy") {
        //Enemyとの衝突処理

    }
}