#include "GameScene.h"

#include "Helper/DefaultSettings.h"

#include <Novice.h>
#include "Object/Player/Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "easing/EasingManager/EasingManager.h"
#include "Helper/InputCenter/InputCenter.h"

#include <format>
#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene()
{
    pCollisionManager_ = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("GameScene", std::bind(&GameScene::DebugWindow, this));
    keys_= InputCenter::GetInstance()->GetKeyPtr();
    preKeys_ = InputCenter::GetInstance()->GetPreKeyPtr();
    pEnemyManager_ = EnemyManager::GetInstance();
}

GameScene::~GameScene()
{
    pCollisionManager_->ClearColliderList();
    DebugManager::GetInstance()->DeleteComponent("GameScene");
    delete pCore_; pCore_ = nullptr;
    delete pNestWallLeft_; pNestWallLeft_ = nullptr;

    for (Enemy* ptr : enemyList_)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pEnemyManager_->SetEnemyList(&enemyList_);


    pCollisionManager_->Initialize();

    pEnemyPopSystem_ = new EnemyPopSystem();
    pEnemyPopSystem_->Load("EnemyPop.csv");

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    uint32_t nestWallWidth = 40u;

    MakeWall(&pNestWallLeft_, "Left", nestWallWidth, DefaultSettings::kGameScreenHeight, { DefaultSettings::kGameScenePosX,DefaultSettings::kGameScenePosY });
    MakeWall(&pNestWallTop_, "Top", DefaultSettings::kGameScreenWidth, nestWallWidth, { DefaultSettings::kGameScenePosX,DefaultSettings::kGameScenePosY });
    MakeWall(&pNestWallRight_, "Right", nestWallWidth, DefaultSettings::kGameScreenHeight, { DefaultSettings::kScreenWidth - nestWallWidth, DefaultSettings::kGameScenePosY });
    MakeWall(&pNestWallBottom_, "Bottom", DefaultSettings::kGameScreenWidth, nestWallWidth, { DefaultSettings::kGameScenePosX ,DefaultSettings::kGameScreenHeight - nestWallWidth });

	pNestWallLeft_->SetHPBarPos(Vector2(float(nestWallWidth + 40), DefaultSettings::kGameScreenHeight / 2.0f + pNestWallTop_->GetHPBarWidth() / 2.0f));
	pNestWallLeft_->SetHPBarRotation(-std::numbers::pi_v<float> / 2.0f);

	pNestWallTop_->SetHPBarPos(Vector2(DefaultSettings::kGameScreenWidth / 2.0f - pNestWallTop_ ->GetHPBarWidth() / 2.0f, float(nestWallWidth + 40)));

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

void GameScene::Update()
{
    /// タイマーが止まっていたらスタートする
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }

    if (keys_[DIK_F3] && !preKeys_[DIK_F3])
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

    pCollisionManager_->CheckAllCollision();


    /// PopSystem
    if (isPop_)
    {
        //Enemy* ptrEnemy = pEnemyPopSystem_->Update(0.01, { 0,0 }, { DefaultSettings::kScreenWidth, DefaultSettings::kScreenHeight });
        //Enemy* ptrEnemy = nullptr;
        //if (keys_[DIK_AT] && !preKeys_[DIK_AT])
        //    ptrEnemy = pEnemyPopSystem_->Spawn({ 0,0 }, { DefaultSettings::kScreenWidth, DefaultSettings::kScreenHeight });
        //if (ptrEnemy)
        //{
        //    /// Enemyの初期化処理
        //    std::stringstream ss;
        //    ss << "0x" << std::hex << reinterpret_cast<uintptr_t>(ptrEnemy);
        //    ptrEnemy->Initialize(ss.str());
        //    ptrEnemy->SetTargetPosition(pPlayer_->GetWorldPosition());
        //    ptrEnemy->RunSetMask();
        //    ptrEnemy->SetEnableLighter(isEnableLighter_);
        //    ptrEnemy->SetBouncePower(Enemy::BounceTarget::Enemy, e2eBouncePower_);
        //    enemyList_.push_back(ptrEnemy);
        //}

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
}

void GameScene::Draw()
{
    // 背景
    Novice::DrawBox(
        DefaultSettings::kGameScenePosX,
        DefaultSettings::kGameScenePosY,
        DefaultSettings::kGameScreenWidth,
        DefaultSettings::kGameScreenHeight,
        0.0f, 0x242424ff, kFillModeSolid
    );

    pNestWallLeft_->Draw();
    pPlayer_->Draw();
    pCore_->Draw();
    pNestWallLeft_->Draw();
    pNestWallTop_->Draw();
    pNestWallRight_->Draw();
    pNestWallBottom_->Draw();
    for (Enemy* ptr : enemyList_) ptr->Draw();

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

void GameScene::MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin)
{
    *_nestWall = new NestWall(_id);
    (*_nestWall)->SetRect(_width, _height, _origin);
    (*_nestWall)->Initialize();
}
