#pragma once
#include <list>
#include "Object/Enemy/Enemy.h"

class EnemyManager
{
public:
    EnemyManager(const EnemyManager&) = delete;
    EnemyManager(const EnemyManager&&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&&) = delete;

    static EnemyManager* GetInstance() { static EnemyManager instance; return &instance; }

    void ChangeState(Enemy::State _state);
    void SetEnemyList(std::list<Enemy*>* _enemylist) { enemyList_ = _enemylist; };
    void SetStateSuction(float _suctionPower);

private:
    EnemyManager() = default;
    ~EnemyManager() = default;

    std::list<Enemy*>* enemyList_;

};