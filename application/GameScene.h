#pragma once
#include "BaseScene.h"

#include "Draw2D.h"
#include "Input.h"

#include "Object/Player/Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "Object/Wall/NestWall.h"
#include "Object/RotateBoard/RotateBoard.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include "GameSystem/EnemyManager/EnemyManager.h"

#include "GameSystem/EnemyPopSystem/EnemyPopSytem.h"
#include "externals/Timer/Timer.h"

#include <Vector2.h>
#include <vector>


class GameScene : public BaseScene
{
public: // メンバ関数

    GameScene();
    ~GameScene();


    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// ImGuiの描画
    /// </summary>
    void DrawImGui() override;

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
    float               e2eBouncePower_     = 0.1f;
    float               e2rbBouncePower_    = 3.5f;
    bool                isDebugEnable_      = true;
    bool                isDebug_            = false;

private: /// 他オブジェクトのデータ
    CollisionManager*   pCollisionManager_  = nullptr;
    EnemyManager*       pEnemyManager_      = nullptr;
    Input*              pInput_             = nullptr;
    Draw2D*             pDraw2D_            = nullptr;

private: /// 非公開メソッド
    void MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin, size_t _offset);

    template<typename T>
    void DeleteIf(T** _object)
    {
        if (!*_object) return;
        if ((*_object)->GetIsDead())
        {
            delete *_object;
            *_object = nullptr;
        }
    }

    /// <summary>
    /// nullptrチェックを行い、安全に削除する
    /// </summary>
    /// <typeparam name="T">型</typeparam>
    /// <param name="_object">オブジェクトのポインタのポインタ</param>
    template<typename T>
    void SafeDelete(T** _object)
    {
        if (!*_object) return;
        delete* _object;
        *_object = nullptr;
    }
};
