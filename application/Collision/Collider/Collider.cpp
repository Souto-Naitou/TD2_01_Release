#include "Collision/Collider/Collider.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include "Helper/ImGuiTemplates/ImGuiTemplates.h"
#include <sstream>

Collider::Collider()
{
}

Collider::~Collider()
{
    DebugManager::GetInstance()->DeleteComponent("Colliders", colliderID_.c_str());
}

const bool Collider::IsRegisteredCollidingPtr(const Collider* _ptr) const
{
    for (auto itr = collidingPtrList_.begin(); itr != collidingPtrList_.end(); ++itr)
    {
        if (_ptr == *itr) return true;
    }
    return false;
}

void Collider::EraseCollidingPtr(const Collider* _ptr)
{
    collidingPtrList_.remove_if([_ptr](const Collider* _pCollider) {
        return _pCollider == _ptr;
    });
    return;
}

void Collider::SetVertices(const std::vector<Vector2>&& _vertices)
{
    verticesCollider_ = _vertices;
}

void Collider::SetVertices(const std::vector<Vector2>* _vertices)
{
    verticesCollider_ = *_vertices;
}

void Collider::SetVertices(Vector2* _vertices, size_t _size)
{
    verticesCollider_.clear();
    for (int i = 0; i < _size; i++)
    {
        verticesCollider_.push_back(*(_vertices + i));
    }
}

void Collider::SetAttribute(uint32_t _attribute)
{
    collisionAttribute_ = _attribute;
}

void Collider::SetMask(uint32_t* _mask)
{
    pCollisionMask_ = _mask;
}

void Collider::OnCollisionTrigger(const Collider* _other)
{
    if (onCollisionTriggerFunction_)
        onCollisionTriggerFunction_(_other);
    return;
}

void Collider::DebugWindow()
{
    auto pFunc = [&]()
    {
        for (auto ptr : collidingPtrList_)
        {
            std::stringstream ss;
            ss << "0x" << std::hex << reinterpret_cast<uintptr_t>(ptr);
            ImGuiTemplate::VariableTableRow(ss.str(), ptr->GetColliderID());
        }
    };

    ImGuiTemplate::VariableTable("Collider", pFunc);
}
