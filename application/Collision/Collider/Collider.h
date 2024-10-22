#pragma once

#include "Helper/Shape.h"
#include "Vector2.h"
#include <vector>
#include <string>
#include <functional>
#include "Object/.Base/BaseObject.h"
#include <list>
#include "Externals/ImGuiDebugManager/DebugManager.h"

class CollisionManager;

class Collider
{
public:

    Collider();
    ~Collider();


    inline  const BaseObject*           GetOwner()                  const       { return owner_; }

    inline  uint32_t                    GetCollisionAttribute()     const       { return collisionAttribute_; }
    inline  uint32_t                    GetCollisionMask()          const       { return *pCollisionMask_;}
    inline  Shape                       GetShape()                  const       { return shape_; }
    inline  unsigned int                GetRadius()                 const       { return radiusCollider_; }
    inline  const std::vector<Vector2>* GetVertices()               const       { return &verticesCollider_; }
    inline  const std::string&          GetColliderID()             const       { return colliderID_; }
    inline  bool                        GetIsEnableLighter()        const       { return enableLighter_; }
    inline  Vector2                     GetPosition()               const       { return position_; }
    inline  const bool                  GetEnable()                 const       { return isEnableCollision_; }

            const bool                  IsRegisteredCollidingPtr(const Collider* _ptr) const;
            void                        EraseCollidingPtr(const Collider* _ptr);



    inline  void                        SetOwner(BaseObject* _owner)            { owner_ = _owner; }
    inline  void                        SetColliderID(const std::string& _id)
    {
        colliderID_ = _id;
        DebugManager::GetInstance()->SetComponent("Colliders", colliderID_, std::bind(&Collider::DebugWindow, this));
    }
    void                                SetVertices(const std::vector<Vector2>&& _vertices);
    void                                SetVertices(const std::vector<Vector2>* _vertices);
    void                                SetVertices(Vector2* _vertices, size_t _size);
    void                                SetAttribute(uint32_t _attribute);
    void                                SetMask(uint32_t* _mask);
    void                                SetOnCollision(const std::function<void(const Collider*)>& _func) { onCollisionFunction_ = _func; }
    void                                SetOnCollisionTrigger(const std::function<void(const Collider*)>& _func) { onCollisionTriggerFunction_ = _func; }
    void                                SetRadius(unsigned int _rad) { radiusCollider_ = _rad; }
    void                                SetPosition(const Vector2& _v) { position_ = _v; }
    void                                SetEnableLighter(bool _flag) { enableLighter_ = _flag; }
    void                                SetEnable(bool _flag) { isEnableCollision_ = _flag; }
    void                                RegisterCollidingPtr(const Collider* _ptr) { collidingPtrList_.push_back(_ptr); }


    inline  void                        OnCollision(const Collider* _other)
    {
        if (onCollisionFunction_)
            onCollisionFunction_(_other);
        return;
    }

    void OnCollisionTrigger(const Collider* _other);

private:

    std::function<void(const Collider*)> onCollisionFunction_;
    std::function<void(const Collider*)> onCollisionTriggerFunction_;

    BaseObject*                     owner_                      = nullptr;
    bool                            isEnableCollision_          = true;                         // 判定をするかどうか
    Shape                           shape_                      = Shape::Polygon;               // 形状
    std::string                     colliderID_                 = {};                           // ID

    std::vector<Vector2>            verticesCollider_           = {};                           // 当たり判定で使用する頂点
    std::list<const Collider*>      collidingPtrList_           = {};                           // 現在あたっているコライダーのリスト

    /// 軽量化用
    unsigned int                    radiusCollider_             = 0u;
    Vector2                         position_                   = {};
    bool                            enableLighter_              = false;

    // 衝突属性(自分)
    uint32_t                        collisionAttribute_         = 0xffffffff;
    // 衝突マスク(相手)
    uint32_t*                       pCollisionMask_             = new uint32_t(0xffffffff);

private:
    void DebugWindow();

};