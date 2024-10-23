#include "EnemyPopSytem.h"
#include "Helper/DefaultSettings.h"
#include <random>
#include "externals/ImGuiDebugManager/DebugManager.h"
#include "Helper/ImGuiTemplates/ImGuiTemplates.h"

EnemyPopSystem::EnemyPopSystem()
{
    csvl_Instance = CSVLoader::GetInstance();
    DebugManager::GetInstance()->SetComponent("EnemyPopSystem", std::bind(&EnemyPopSystem::DebugWindow, this));
}

void EnemyPopSystem::Initialize()
{

}

void EnemyPopSystem::Load(std::string _fileName)
{
    pCSVData = csvl_Instance->LoadFile(_fileName);
}

Enemy* EnemyPopSystem::Spawn(const Vector2& _start, const Vector2& _end)
{
    Enemy* pEnemy = new Enemy();
    Vector2 result = {};
    Vector2 range = _end - _start;

    result.x = static_cast<float>(rand() % static_cast<int>(range.x));
    result.y = static_cast<float>(rand() % static_cast<int>(range.y));
    pEnemy->SetWorldPosition(result);

    return pEnemy;
}

Enemy* EnemyPopSystem::Spawn(const Vector2& _point, float _range)
{
    Enemy* pEnemy = new Enemy();
    Vector2 result = {};
    float halfRange = _range * 0.5f;
    Vector2 start =
    {
        _point.x - halfRange,
        _point.y - halfRange
    };

    result.x = static_cast<float>(rand() % static_cast<int>(_range)) + _point.x;
    result.y = static_cast<float>(rand() % static_cast<int>(_range)) + _point.y;
    pEnemy->SetWorldPosition(result);

    return pEnemy;
}

void EnemyPopSystem::SpawnFromCSV(std::list<Enemy*>& _enemyList, Player* _pPlayer, bool _isEnableLighter, float _bouncePower)
{
    if (!timer_.GetIsStart()) timer_.Start();
    if (isFirst_)
    {
        if (timer_.GetNow() > 3.0)
        {
            isFirst_ = false;
        }
        else return;
    }
    else if (timer_.GetNow() < intervalSpawn) return;

    timer_.Reset();
    timer_.Start();

    pPlayer_ = _pPlayer;

    while (true)
    {
        CSVData::iterator itr = pCSVData->begin();
        int lineNum = rand() % pCSVData->size();
        // イテレータをlineNum分進める
        std::advance(itr, lineNum);
        CSVLine& line = *itr;
        std::string direction;
        int count = 0;
        line[0].seekg(0);
        line[1].seekg(0);
        line[1] >> count;
        line[0] >> direction;

        if (direction == "Top")
        {
            if (!pNestWallTop_) continue;
            for (int i = 0; i < count; ++i)
            {
                Vector2 point =
                {
                    static_cast<float>(rand() % DefaultSettings::kGameScreenWidth) + DefaultSettings::kGameScenePosX,
                    static_cast<float>(rand() % 50) + DefaultSettings::kGameScenePosY - 110
                };
                Enemy* pEnemy = Spawn(point, 50);
                EnemyInitialize(pEnemy, _isEnableLighter, _bouncePower);
                _enemyList.push_back(pEnemy);
            }
        }
        else if (direction == "Left")
        {
            if (!pNestWallLeft_) continue;
            for (int i = 0; i < count; ++i)
            {
                Vector2 point =
                {
                    static_cast<float>(rand() % 50) + DefaultSettings::kGameScenePosX - 110,
                    static_cast<float>(rand() % DefaultSettings::kGameScreenHeight) + DefaultSettings::kGameScenePosY
                };
                Enemy* pEnemy = Spawn(point, 50);
                EnemyInitialize(pEnemy, _isEnableLighter, _bouncePower);
                _enemyList.push_back(pEnemy);
            }
        }
        else if (direction == "Right")
        {
            if (!pNestWallRight_) continue;
            for (int i = 0; i < count; ++i)
            {
                Vector2 point =
                 {
                    static_cast<float>(rand() % 50) + DefaultSettings::kGameScreenWidth + 10,
                    static_cast<float>(rand() % DefaultSettings::kGameScreenHeight) + DefaultSettings::kGameScenePosY
                };
                Enemy* pEnemy = Spawn(point, 50);
                EnemyInitialize(pEnemy, _isEnableLighter, _bouncePower);
                _enemyList.push_back(pEnemy);
            }
        }
        else if (direction == "Bottom")
        {
            if (!pNestWallBottom_) continue;
            for (int i = 0; i < count; ++i)
            {
                Vector2 point =
                {
                    static_cast<float>(rand() % DefaultSettings::kGameScreenWidth) + DefaultSettings::kGameScenePosX,
                    static_cast<float>(rand() % 50) + DefaultSettings::kGameScreenHeight + 10
                };
                Enemy* pEnemy = Spawn(point, 50);
                EnemyInitialize(pEnemy, _isEnableLighter, _bouncePower);
                _enemyList.push_back(pEnemy);
            }
        }
        break;
    }

}

Enemy* EnemyPopSystem::Update(double _interval, const Vector2& _start, const Vector2& _end)
{
    Enemy* resultPtr = nullptr;

    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }

    if (timer_.GetNow() > _interval)
    {
        resultPtr = Spawn(_start, _end);
        timer_.Reset();
    }

    return resultPtr;
}

void EnemyPopSystem::EnemyInitialize(Enemy* _pEnemy, bool _isEnableLighter, float _bouncePower)
{
    std::stringstream ss;
    ss << "0x" << std::hex << reinterpret_cast<uintptr_t>(_pEnemy);
    _pEnemy->Initialize(ss.str());
    _pEnemy->SetTargetPosition(pPlayer_->GetWorldPosition());
    _pEnemy->RunSetMask();
    _pEnemy->SetEnableLighter(_isEnableLighter);
    _pEnemy->SetBouncePower(Enemy::BounceTarget::Enemy, _bouncePower);
}

void EnemyPopSystem::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("timer_", timer_.GetNow());
    };

    ImGuiTemplate::VariableTable("EnemyPopSystem", pFunc);
}
