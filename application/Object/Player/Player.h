#pragma once

#include "Input.h"
#include "Object/.Base/BaseObject.h"
#include "Draw2D.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include "externals/easing/Easing.h"
#include <Vector2.h>
#include "Object/RotateBoard/RotateBoard.h"
#include "Collision/Collider/Collider.h"
#include "GameSystem/EnemyManager/EnemyManager.h"

#include <vector>
#include <chrono>
#include <memory>

class Player : public BaseObject
{
public:

    Player();
    ~Player();

    void            Initialize();
    void            RunSetMask();
    void            Update();
    void            Draw();
    void            SetEnableLighter(bool _flag) { collider_.SetEnableLighter(_flag); }
    bool            IsAttack() const { return isAttack_; }
    void            OnCollision(const Collider* _other);
    Collider*       GetCollider() { return &collider_; }
    float           GetAttackMultiply() const { return latestAttackMultiply_; }

private:

    Collider        collider_                   = {};
    RotateBoard*    pRotateBoard_               = nullptr;

    float           radius_default_             = 100.0f;
    float           radius_min_                 = 20.0f;
    float           radius_current_             = 0.0f;
    float           radius_timeRelease_         = 0.0f;
    float           latestAttackMultiply_       = 0.0f;

    size_t          resolution_                 = 4u;
    bool            isAttack_                   = false;

    std::vector<Vector2>                    vertices_           = {};
    std::chrono::system_clock::time_point   startTime_          = {};
    std::unique_ptr<Easing>                 pEasingBoxResize_   = nullptr;
    std::unique_ptr<Easing>                 pEasingBoxTemp_     = nullptr;

private: /// 他オブジェクトから借りるデータ
    CollisionManager*                       pCollisionManager_  = nullptr;
    EnemyManager*                           pEnemyManager_      = nullptr;
    Draw2D*                                 pDraw2D_            = nullptr;
    Input*                                  pInput_             = nullptr;

private: /// 非公開メソッド
    void DebugWindow();
};