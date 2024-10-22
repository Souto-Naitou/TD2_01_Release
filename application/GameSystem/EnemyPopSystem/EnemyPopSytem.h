#pragma once

#include "Object/Enemy/Enemy.h"
#include "GameSystem/BasePopSytem.h"
#include "externals/easing/CSVLoader.h"
#include <Vector2.h>
#include <list>
#include "Object/Player/Player.h"
#include "externals/Timer/Timer.h"

class EnemyPopSystem : public BasePopSystem
{
public:

    EnemyPopSystem();

    void Initialize();

    void Load(std::string _fileName);

    Enemy* Spawn(const Vector2& _start, const Vector2& _end);
    Enemy* Spawn(const Vector2& _point, float _range);

    void SpawnFromCSV(std::list<Enemy*>& _enemyList, Player* _pPlayer, bool _isEnableLighter, float _bouncePower);

    Enemy* Update(double _interval, const Vector2& _start, const Vector2& _end);

private:
    CSVLoader* csvl_Instance = nullptr;
    CSVData* pCSVData = nullptr;
    double intervalSpawn = 5.0;
    Player* pPlayer_ = nullptr;

private:
    void EnemyInitialize(Enemy* _pEnemy, bool _isEnableLighter, float _bouncePower);
    void DebugWindow();
};