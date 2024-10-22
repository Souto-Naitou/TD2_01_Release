#pragma once

#include "Object/.Base/BaseObject.h"
#include <Rect2.h>
#include "Collision/Collider/Collider.h"

class Core : public BaseObject
{
public:

    Core();
    ~Core();

    void Initialize();
    void RunSetMask();
    void Update();
    void Draw();

    void SetEnableLighter(bool _flag) { collider_.SetEnableLighter(_flag); }

    // コールバック関数
    void OnCollision(const Collider* _other);

    void DebugWindow();

    Collider* GetCollider() { return &collider_; }

private:
    Rect2 boxCore_;
    Collider collider_;
    CollisionManager* pCollisionManager; // シングルトンインスタンス
};