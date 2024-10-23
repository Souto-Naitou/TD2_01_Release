#include "Enemy.h"

#include "Draw2D.h"
#include "imgui.h"
#include "Helper/ImGuiTemplates/ImGuiTemplates.h"
#include "Externals/ImGuiDebugManager/DebugManager.h"
#include "Collision/CollisionManager/CollisionManager.h"

#include "Helper/DefaultSettings.h"
#include "Object/Player/Player.h"
#include "Object/RotateBoard/RotateBoard.h"
#include "Audio.h"

float Enemy::bouncePower_enemy_ = 0.25f;
float Enemy::bouncePower_rotateBoard_ = 3.0f;
float Enemy::bouncePower_nestWall_ = 3.0f;

Enemy::~Enemy()
{
    pCollisionManager_->DeleteCollider(&collider_);
    DebugManager::GetInstance()->DeleteComponent("Enemy", idx_.c_str());
}

void Enemy::Initialize(std::string _idx)
{
    /// コライダーの登録
    pCollisionManager_ = CollisionManager::GetInstance();
    pCollisionManager_->RegisterCollider(&collider_);


    /// デバッグ用ウィンドウの登録
    idx_ = _idx;                                // 引数から受け取った数を文字に
    objectID_ = "Enemy" + idx_;                 //"Enemy1", "Enemy2" など
    DebugManager::GetInstance()->SetComponent("Enemy", idx_, std::bind(&Enemy::DebugWindow, this));


    /// 頂点の計算に必要なデータを入力
    radius_ = 20.0f;
    ellipseAB_ = { 20.0f ,10.0f };              // ax^2 + by^2 = 1


    collider_.SetColliderID("Enemy");                       // コライダーのID
    moveSpeed_          = 1.0f;                             // 移動スピード
    moveSpeed_sucked_   = 10.0f;                            // 吸い込み時加算スピード
    hp_                 = 3;                                // HP
    color_              = { 1.0f, 0.0f, 0.0f, 1.0f };       // 色

    /// コライダーの設定
    collider_.SetOwner(this);
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("Enemy"));
    collider_.SetRadius(static_cast<int>(radius_));
    collider_.SetVertices(vertices_, 3);
    // Colliderにポインタを渡す
    collider_.SetOnCollision(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
    collider_.SetOnCollisionTrigger(std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1));

	// サウンドの読み込み
	hitWallSH_ = Audio::GetInstance()->LoadWaveFile("hit3.wav");
	hitRotateBoardSH_ = Audio::GetInstance()->LoadWaveFile("hit.wav");
}

void Enemy::Update()
{
    /// 早期リターン
    if (isDead_) return;
    if (outScreenPadding_)
    {
        if (position_.x < -outScreenPadding_ ||
            position_.x > DefaultSettings::kScreenWidth + outScreenPadding_ ||
            position_.y < -outScreenPadding_ ||
            position_.y > DefaultSettings::kScreenHeight + outScreenPadding_)
        {
            collider_.SetEnable(false);
            if (isBounce_) isDead_ = true;
            if (!isOutOfScreen_)
            {
                acceleration_ = {};
                velocity_ = {};
                isOutOfScreen_ = true;
                return;
            }
        }
        else
        {
            collider_.SetEnable(true);
            isOutOfScreen_ = false;
        }
    }

    if (hp_ <= 0) isDead_ = true;

    // Playerに攻撃されたら色を変更
    if (isBounce_) color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

    if (state_ == State::Normal) suctionPower_ = 0.0f;


    /// 速度の更新
    distanceToTarget = positionTarget_ - position_;
    if (distanceToTarget.Length() > 0)
    {
        Vector2 normalDist2Target = distanceToTarget.Normalize();
        velocity_move = normalDist2Target * moveSpeed_;
        velocity_move += normalDist2Target * moveSpeed_sucked_ * suctionPower_;
    }

    /// 方向を変更
    if ((distanceToTarget.x != 0 || distanceToTarget.y != 0))
    {
        rotation_ = (velocity_ + velocity_move).Normalize().Theta();
    }



    /// ** ここより上ではPositionを更新しない **

    /// 頂点の計算
    float theta = 0;
    Vector2 result = {};
    for (int i = 0; i < 3; i += 1)
    {
        theta += 2.0f / 3 * 3.141592f;
        result.x = ellipseAB_.x * std::cosf(theta);
        result.y = ellipseAB_.y * std::sinf(theta);
        vertices_[i] = result.Rotated(rotation_) + position_;
    }

    collider_.SetVertices(vertices_, 3);

    velocity_ += acceleration_;

    velocity_.Lerp(velocity_, velocity_move, 0.01f);

    position_ += velocity_;

    // 計算終わったら初期化
    acceleration_ = {};

    // コライダーに座標を送る
    if (collider_.GetIsEnableLighter()) collider_.SetPosition(position_);

    return;
}

void Enemy::Draw()
{

#ifndef _DEBUG
    /// 早期リターン (Debug時は確認のため無効)
    if (isDead_) return;
#endif // !_DEBUG
    if (isOutOfScreen_) return;

    /// 3頂点の描画 (Enemy本体)

    Draw2D::GetInstance()->DrawTriangle(vertices_[0], vertices_[1], vertices_[2], color_);

    return;
}

void Enemy::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask(objectID_));
}

void Enemy::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("Velocity", velocity_);
        ImGuiTemplate::VariableTableRow("Acceleration", acceleration_);
        ImGuiTemplate::VariableTableRow("DistanceToTarget", distanceToTarget);
        ImGuiTemplate::VariableTableRow("isDead_", isDead_);
        ImGuiTemplate::VariableTableRow("rotation_", rotation_);
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
    };

    ImGui::DragFloat("移動速度", &moveSpeed_, 0.01f, 0.0f);

    ImGuiTemplate::VariableTable("Enemy", pFunc);

    return;
}

void Enemy::SetState(State _state, float _val)
{
    SetState(_state);
    switch (_state)
    {
    case Enemy::State::Normal:
        break;
    case Enemy::State::Suck:
        suctionPower_ = _val;
        break;
    default:
        break;
    }
}

void Enemy::OnCollision(const Collider* _other)
{
    std::string otherID = _other->GetColliderID();

    /// Playerとの衝突後
    if (otherID == "Player")
    {
        const Player* pPlayer = static_cast<const Player*>(_other->GetOwner());
        if (pPlayer->IsAttack())
        {
            float attackMultiply = pPlayer->GetAttackMultiply();
            float bouncePower = bouncePowerMax_ * attackMultiply;

            // 反発を加える
            if (attackMultiply > 0.1f)
            {
                velocity_ = {};
                acceleration_ = -distanceToTarget.Normalize() * bouncePower;
            }

            isBounce_ = true;     //ぶっ飛びフラグオン
        }
    }
    /// Coreとの衝突後
    else if (otherID == "Core")
    {
        // エネミーのデスフラグをオンに
        isDead_ = true;
    }
    /// 敵同士の衝突後
    else if (otherID == "Enemy")
    {
        // 敵同士の位置を取得
        const Enemy* otherEnemy = static_cast<const Enemy*>(_other->GetOwner());
        Vector2 otherPosition = otherEnemy->GetWorldPosition();

        // 短い距離を計算
        Vector2 direction = position_ - otherPosition;

        // 反発ベクトルを計算
        if (direction.Length() > 0)
        {
            // 反発を加える
            acceleration_ += direction.Normalize() * bouncePower_enemy_;
        }
        // 連鎖処理
        if (otherEnemy->GetIsBounce()) isBounce_ = true;

    }
    /// 巣壁との衝突後
    else if (otherID == "NestWall")
    {
    }
    /// 回転板との衝突後
    else if (otherID == "RotateBoard")
    {
        const RotateBoard* pRotateBoard = static_cast<const RotateBoard*>(_other->GetOwner());

        const Vector2* rbVertices1 = pRotateBoard->GetVertices(1);
        const Vector2* rbVertices2 = pRotateBoard->GetVertices(2);

        if (pRotateBoard->GetIsCorner())
        {
            Vector2 rbEdge1 = rbVertices1[0] - rbVertices1[1];
            Vector2 rbEdge2 = rbVertices2[1] - rbVertices2[0];
            if (rbEdge1.Cross(position_ - rbVertices1[1]) > 0)
            {
                velocity_ = {};
                acceleration_ = rbEdge1.Perpendicular().Normalize() * bouncePower_rotateBoard_;
            }
            if (rbEdge2.Cross(position_ - rbVertices2[1]) > 0)
            {
                velocity_ = {};
                acceleration_ = rbEdge2.Perpendicular().Normalize() * bouncePower_rotateBoard_;
            }
        }
        else
        {
            Vector2 rbEdge = rbVertices1[0] - rbVertices1[1];

            if (rbEdge.Cross(position_ - rbVertices1[1]) < 0) return;
            velocity_ = {};
            acceleration_ = rbEdge.Perpendicular().Normalize() * bouncePower_rotateBoard_;
        }
    }
}

void Enemy::OnCollisionTrigger(const Collider* _other)
{
    if (_other->GetColliderID() == "NestWall")
    {
        if (!isBounce_) return;
        auto otherVertices = _other->GetVertices();
        Vector2 edge = (*otherVertices)[2] - (*otherVertices)[1];

        velocity_ = {};
        acceleration_ = edge.Perpendicular().Normalize() * bouncePower_nestWall_;

        hp_--;

		// ヒットサウンド再生
		Audio::GetInstance()->PlayWave(hitWallSH_, 0.3f);
    }

    if (_other->GetColliderID() == "RotateBoard") {
        // ヒットサウンド再生
        Audio::GetInstance()->PlayWave(hitRotateBoardSH_, 0.3f);
    }
}

void Enemy::SetEnableLighter(bool _flag)
{
    collider_.SetEnableLighter(_flag);
    if (_flag)
    {
        outScreenPadding_ = 200;
    }
}

void Enemy::SetBouncePower(BounceTarget _target, float _power)
{
    switch (_target)
    {
    case Enemy::BounceTarget::Enemy:
        bouncePower_enemy_ = _power;
        break;
    case Enemy::BounceTarget::Player:
        break;
    case Enemy::BounceTarget::RotateBoard:
        bouncePower_rotateBoard_ = _power;
        break;
    }
}
