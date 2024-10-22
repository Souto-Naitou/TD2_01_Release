#pragma once

#include "Object/Player/Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "Object/Wall/NestWall.h"
#include "Object/RotateBoard/RotateBoard.h"
#include "Collision/CollisionManager.h"
#include "GameSystem/EnemyManager/EnemyManager.h"

#include "GameSystem/EnemyPopSystem/EnemyPopSytem.h"
#include "Timer/Timer.h"

#include <Vector2.h>
#include <vector>

class GameScene
{
public:
    GameScene();
    ~GameScene();

    void Initialize();
    void Update();
    void Draw();

    void DebugWindow();

private: /// 非公開データ
    Player*             pPlayer_            = nullptr;
    Core*               pCore_              = nullptr;
    NestWall*           pNestWallLeft_      = nullptr;
    NestWall*           pNestWallTop_       = nullptr;
    NestWall*           pNestWallRight_     = nullptr;
    NestWall*           pNestWallBottom_    = nullptr;
    EnemyPopSystem*     pEnemyPopSystem_    = nullptr;
    std::list<Enemy*>   enemyList_          = {};
    Timer               timer_              = {};
    bool                isEnableLighter_    = false;
    bool                isPop_              = false;
    float               e2eBouncePower_     = 2.0f;
    float               e2rbBouncePower_     = 2.0f;
    char*               preKeys_            = nullptr;
    char*               keys_               = nullptr;
    bool                isDebugEnable_      = true;

private: /// 他オブジェクトのデータ
    CollisionManager*   pCollisionManager_  = nullptr;
    EnemyManager*       pEnemyManager_      = nullptr;

private: /// 非公開メソッド

    void MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin);
};