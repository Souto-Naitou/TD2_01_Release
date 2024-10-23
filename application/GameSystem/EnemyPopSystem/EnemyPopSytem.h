#pragma once

#include "Object/Enemy/Enemy.h"
#include "GameSystem/BasePopSytem.h"
#include "externals/easing/CSVLoader.h"
#include <Vector2.h>
#include <list>
#include "Object/Player/Player.h"
#include "externals/Timer/Timer.h"
#include "Object/Wall/NestWall.h"

class EnemyPopSystem : public BasePopSystem
{
public:

    EnemyPopSystem();

    void Initialize();

    void Load(std::string _fileName);

    Enemy* Spawn(const Vector2& _start, const Vector2& _end);
    Enemy* Spawn(const Vector2& _point, float _range);

    void SetNestWallTop(NestWall* _pNestWall) { pNestWallTop_ = _pNestWall; }
    void SetNestWallRight(NestWall* _pNestWall) { pNestWallRight_ = _pNestWall; }
    void SetNestWallBottom(NestWall* _pNestWall) { pNestWallBottom_ = _pNestWall; }
    void SetNestWallLeft(NestWall* _pNestWall) { pNestWallLeft_ = _pNestWall; }

    void SpawnFromCSV(std::list<Enemy*>& _enemyList, Player* _pPlayer, bool _isEnableLighter, float _bouncePower);

    Enemy* Update(double _interval, const Vector2& _start, const Vector2& _end);

private:
    CSVLoader* csvl_Instance = nullptr;
    CSVData* pCSVData = nullptr;
    double intervalSpawn = 7.0;
    Player* pPlayer_ = nullptr;
    bool isFirst_ = true;

    NestWall* pNestWallTop_ = nullptr;
    NestWall* pNestWallRight_ = nullptr;
    NestWall* pNestWallBottom_ = nullptr;
    NestWall* pNestWallLeft_ = nullptr;

private:
    void EnemyInitialize(Enemy* _pEnemy, bool _isEnableLighter, float _bouncePower);
    void DebugWindow();
};