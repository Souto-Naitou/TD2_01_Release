#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "TextureManager.h"
#include "DebugCamera.h"

#include "Helper/DefaultSettings.h"
#include "Object/Player/Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "externals/ImGuiDebugManager/DebugManager.h"
#include "externals/easing/EasingManager/EasingManager.h"


#include <format>
#include <cmath>
#include <numbers>


#ifdef _DEBUG
#include "ImGui.h"
#include "DebugCamera.h"
#endif

GameScene::GameScene()
{
    DebugManager::GetInstance()->SetComponent("GameScene", std::bind(&GameScene::DebugWindow, this));

    pDraw2D_            = Draw2D::GetInstance();
    pInput_             = Input::GetInstance();
    pCollisionManager_  = CollisionManager::GetInstance();
    pEnemyManager_      = EnemyManager::GetInstance();
}

GameScene::~GameScene()
{
    DebugManager::GetInstance()->DeleteComponent("GameScene");
    pCollisionManager_->ClearColliderList();
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    DebugCamera::GetInstance()->Initialize();
    DebugCamera::GetInstance()->Set3D();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG
    /// ================================== ///
    ///              初期化処理              ///
    /// ================================== ///

    pEnemyManager_->SetEnemyList(&enemyList_);


    pCollisionManager_->Initialize();

    pEnemyPopSystem_ = new EnemyPopSystem();
    pEnemyPopSystem_->Load("EnemyPop.csv");

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    uint32_t nestWallWidth = 40u;

    MakeWall(&pNestWallLeft_, "Left", nestWallWidth, DefaultSettings::kGameScreenHeight, { DefaultSettings::kGameScenePosX,DefaultSettings::kGameScenePosY }, 2);
    MakeWall(&pNestWallTop_, "Top", DefaultSettings::kGameScreenWidth, nestWallWidth, { DefaultSettings::kGameScenePosX,DefaultSettings::kGameScenePosY },1);
    MakeWall(&pNestWallRight_, "Right", nestWallWidth, DefaultSettings::kGameScreenHeight, { DefaultSettings::kScreenWidth - nestWallWidth, DefaultSettings::kGameScenePosY}, 0);
    MakeWall(&pNestWallBottom_, "Bottom", DefaultSettings::kGameScreenWidth, nestWallWidth, { DefaultSettings::kGameScenePosX ,DefaultSettings::kGameScreenHeight - nestWallWidth }, 3);

    pNestWallLeft_->SetHPBarPos(Vector2(float(nestWallWidth + 40), DefaultSettings::kGameScreenHeight / 2.0f + pNestWallTop_->GetHPBarWidth() / 2.0f));
    pNestWallLeft_->SetHPBarRotation(-std::numbers::pi_v<float> / 2.0f);

    pNestWallTop_->SetHPBarPos(Vector2(DefaultSettings::kGameScreenWidth / 2.0f - pNestWallTop_->GetHPBarWidth() / 2.0f, float(nestWallWidth + 40)));

    pNestWallRight_->SetHPBarPos(Vector2(DefaultSettings::kScreenWidth - pNestWallTop_->GetHPBarWidth() / 2.0f, DefaultSettings::kGameScreenHeight / 2.0f + pNestWallTop_->GetHPBarWidth() / 2.0f));
    pNestWallRight_->SetHPBarRotation(-std::numbers::pi_v<float> / 2.0f);

    pNestWallBottom_->SetHPBarPos(Vector2(DefaultSettings::kGameScreenWidth / 2.0f - pNestWallTop_->GetHPBarWidth() / 2.0f, DefaultSettings::kGameScreenHeight - pNestWallTop_->GetHPBarWidth() / 2.0f));

    /// マスクの生成にアトリビュートを使用するためInitialize後に行う
    pPlayer_->RunSetMask();

    pCore_->RunSetMask();
    pNestWallLeft_->RunSetMask();
    pNestWallTop_->RunSetMask();
    pNestWallRight_->RunSetMask();
    pNestWallBottom_->RunSetMask();

    timer_.Start();

}

void GameScene::Finalize()
{
    delete pCore_; pCore_ = nullptr;
    delete pNestWallLeft_; pNestWallLeft_ = nullptr;

    for (Enemy* ptr : enemyList_)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void GameScene::Update()
{
#ifdef _DEBUG
    if (pInput_->TriggerKey(DIK_F1))
    {
        isDebug_ = !isDebug_;
    }

    if (isDebug_)
    {
        DebugCamera::GetInstance()->Update();
    }
#endif
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///


    /// タイマーが止まっていたらスタートする
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }

    if (pInput_->TriggerKey(DIK_F3))
    {
        isDebugEnable_ = isDebugEnable_ ? false : true;
        DebugManager::GetInstance()->SetDisplay(isDebugEnable_);
        EasingManager::GetInstance()->SetDisplayUI(isDebugEnable_);
    }


    /// 各オブジェクトの更新処理呼出
    pPlayer_->Update();
    pCore_->Update();
    pNestWallLeft_->Update();
    pNestWallTop_->Update();
    pNestWallRight_->Update();
    pNestWallBottom_->Update();
    for (Enemy* ptr : enemyList_) ptr->Update();

    /// 当たり判定処理
    pCollisionManager_->CheckAllCollision();


    /// PopSystem
    if (isPop_)
    {
        pEnemyPopSystem_->SpawnFromCSV(enemyList_, pPlayer_, isEnableLighter_, e2eBouncePower_);
    }

    /// Delete処理ここから
    for (std::list<Enemy*>::iterator itr = enemyList_.begin(); itr != enemyList_.end();)
    {
        if (!(*itr)->GetIsDead())
        {
            itr++;
            continue;
        }
        delete *itr;
        itr = enemyList_.erase(itr);
    }



    // シーン遷移
    if (Input::GetInstance()->TriggerKey(DIK_RETURN))
    {
        SceneManager::GetInstance()->ChangeScene("title");
    }
}

void GameScene::Draw()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///

    // 背景
    pDraw2D_->DrawBox(
        Vector2(DefaultSettings::kGameScenePosX, DefaultSettings::kGameScenePosY),
        Vector2(DefaultSettings::kGameScreenWidth, DefaultSettings::kGameScreenHeight),
        0.0f, Vector4(0.01f, 0.01f, 0.01f, 1.0f)
    );

    pPlayer_->Draw();
    pCore_->Draw();
    pNestWallLeft_->Draw();
    pNestWallTop_->Draw();
    pNestWallRight_->Draw();
    pNestWallBottom_->Draw();
    for (Enemy* ptr : enemyList_) ptr->Draw();
}

void GameScene::DrawImGui()
{
#ifdef _DEBUG

#endif // DEBUG
}

void GameScene::DebugWindow()
{
    if (ImGui::Checkbox("Enable Lag Reduction (beta)", &isEnableLighter_))
    {
        pPlayer_->SetEnableLighter(isEnableLighter_);
        pCore_->SetEnableLighter(isEnableLighter_);
        for (Enemy* enemy : enemyList_)
        {
            enemy->SetEnableLighter(isEnableLighter_);
        }
    }
    ImGui::Checkbox("EnemyPop", &isPop_);

    ImGui::Text("Enemy to Enemy Bounce Power");
    if (ImGui::DragFloat("## PowerE2E", &e2eBouncePower_, 0.01f))
    {
        Enemy::SetBouncePower(Enemy::BounceTarget::Enemy, e2eBouncePower_);
    }

    ImGui::Text("Enemy to RotateBoard Bounce Power");
    if (ImGui::DragFloat("## PowerE2RB", &e2rbBouncePower_, 0.01f))
    {
        Enemy::SetBouncePower(Enemy::BounceTarget::RotateBoard, e2rbBouncePower_);
    }

    if (ImGui::Button("Delete All Enemies"))
    {
        for (Enemy* enemy : enemyList_)
        {
            enemy->SetIsDead(true);
        }
    }
}

void GameScene::MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin, size_t _offset)
{
    *_nestWall = new NestWall(_id);
    (*_nestWall)->SetRect(_width, _height, _origin, _offset);
    (*_nestWall)->Initialize();
}
