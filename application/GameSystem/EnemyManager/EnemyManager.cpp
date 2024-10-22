#include "EnemyManager.h"

void EnemyManager::ChangeState(Enemy::State _state)
{
    for (auto enemy : *enemyList_)
    {
        enemy->SetState(_state);
    }
}

void EnemyManager::SetStateSuction(float _suctionPower)
{
    for (auto enemy : *enemyList_)
    {
        enemy->SetState(Enemy::State::Suck, _suctionPower);
    }
}
