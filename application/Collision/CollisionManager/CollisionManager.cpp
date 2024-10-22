#include "CollisionManager.h"
#include "Helper/Shape.h"
#include <Vector2.h>
#include "externals/ImGuiDebugManager/DebugManager.h"

void CollisionManager::Initialize()
{
    DebugManager::GetInstance()->SetComponent("#Window", "CollisionManager", std::bind(&CollisionManager::DebugWindow, this));
}

void CollisionManager::CheckAllCollision()
{
    collisionNames_.clear();
    countCheckCollision_ = 0ui32;
    countWithoutFilter_ = 0ui32;
    countWithoutLighter = 0ui32;

    auto itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA)
    {
        auto itrB = itrA + 1;
        for (; itrB != colliders_.end(); ++itrB)
        {
            CheckCollisionPair(*itrA, *itrB);
        }
    }
}

void CollisionManager::RegisterCollider(Collider* _collider)
{
    colliders_.push_back(_collider);
}

void CollisionManager::ClearColliderList()
{
    colliders_.clear();
}

void CollisionManager::DeleteCollider(Collider* _collider)
{
    for (int i = 0; i < colliders_.size(); i++)
    {
        if (colliders_[i] == _collider)
        {
            colliders_.erase(colliders_.begin() + i);
        }
    }
}

uint32_t CollisionManager::GetNewAttribute(std::string _id)
{
    if (attributeList_.size() == 0)
    {
        attributeList_.push_back({ _id, 1 });
        return 1;
    }
    for (auto& attributePair : attributeList_)
    {
        if (attributePair.first.compare(_id) == 0)
        {
            return attributePair.second;
        }
    }

    uint32_t result = static_cast<uint32_t>(attributeList_.back().second << 1);

    attributeList_.push_back({ _id, result});

    return attributeList_.back().second;
}

void CollisionManager::DebugWindow()
{
    ImGui::Text("判定回数 : %u回", countCheckCollision_);
    ImGui::Text("フィルターされた回数 : %u回", countWithoutFilter_ - countWithoutLighter);
    ImGui::Text("軽量化された回数 : %u回", countWithoutLighter - countCheckCollision_);

    if (ImGui::BeginTable("Collided list", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("ColliderA");
        ImGui::TableSetupColumn("ColliderB");
        ImGui::TableHeadersRow();

        for (auto& cpair : collisionNames_)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::Text(cpair.first.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(cpair.second.c_str());
        }

        ImGui::EndTable();
    }
}

void CollisionManager::CheckCollisionPair(Collider* _colA, Collider* _colB)
{
    // 衝突しているかどうか
    bool isCollide = true;

    if (!_colA->GetEnable() || !_colB->GetEnable())
    {
        _colA->EraseCollidingPtr(_colB);
        _colB->EraseCollidingPtr(_colA);
        return;
    }
    countWithoutFilter_++;

    // 衝突フィルタリング
    bool fillterFlag =
        !(_colA->GetCollisionAttribute() & _colB->GetCollisionMask()) ||
        !(_colB->GetCollisionAttribute() & _colA->GetCollisionMask());
    if (fillterFlag) return;

    // 形状条件
    if (_colA->GetShape() == Shape::Polygon && _colB->GetShape() == Shape::Polygon)
    {
        const std::vector<Vector2>* pVerticesA = _colA->GetVertices();
        const std::vector<Vector2>* pVerticesB = _colB->GetVertices();

        if (!pVerticesA->size() || !pVerticesB->size())
        {
            _colA->EraseCollidingPtr(_colB);
            _colB->EraseCollidingPtr(_colA);
            return;
        }

        countWithoutLighter++;

        /// ラグ軽減のため、半径で判定とって早期リターン (ただし設定されていたら)
        if (_colA->GetRadius() && _colB->GetRadius() && _colA->GetIsEnableLighter() && _colB->GetIsEnableLighter())
        {
            Vector2 distanceAB = _colB->GetPosition() - _colA->GetPosition();
            uint32_t radiusAB = _colA->GetRadius() + _colB->GetRadius();
            if (distanceAB.LengthWithoutRoot() > static_cast<float>(radiusAB * radiusAB)) return;
        }

        countCheckCollision_++;

        // Aのすべての分離軸でチェック
        for (size_t i = 0; i < pVerticesA->size(); i++)
        {
            Vector2 edge = (*pVerticesA)[i] - (*pVerticesA)[(i + 1) % pVerticesA->size()];
            Vector2 axis = edge.Perpendicular().Normalize();
            // col1とcol2の投影をして、最小値・最大値を求める
            float minA, maxA, minB, maxB;
            ProjectShapeOnAxis(pVerticesA, axis, minA, maxA);
            ProjectShapeOnAxis(pVerticesB, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
            {
                isCollide = false;
                break;
            }
        }
        if (isCollide)
        {
            for (size_t i = 0; i < pVerticesB->size(); i++)
            {
                Vector2 edge = (*pVerticesB)[i] - (*pVerticesB)[(i + 1) % pVerticesB->size()];
                Vector2 axis = edge.Perpendicular().Normalize();
                // col1とcol2の投影をして、最小値・最大値を求める
                float minA, maxA, minB, maxB;
                ProjectShapeOnAxis(pVerticesA, axis, minA, maxA);
                ProjectShapeOnAxis(pVerticesB, axis, minB, maxB);

                if (maxA < minB || maxB < minA)
                {
                    isCollide = false;
                    break;
                }
            }
        }
    }

    if (isCollide)
    {
        _colA->OnCollision(_colB);
        _colB->OnCollision(_colA);

        if (!_colA->IsRegisteredCollidingPtr(_colB) && !_colB->IsRegisteredCollidingPtr(_colA))
        {
            _colA->RegisterCollidingPtr(_colB);
            _colB->RegisterCollidingPtr(_colA);
            _colA->OnCollisionTrigger(_colB);
            _colB->OnCollisionTrigger(_colA);
        }

        collisionNames_.push_back({ _colA->GetColliderID(), _colB->GetColliderID() });
    }
    else
    {
        // あたっていない場合、CollidingPtrをチェックし該当する場合ポップ
        _colA->EraseCollidingPtr(_colB);
        _colB->EraseCollidingPtr(_colA);
    }
    return;
}

void CollisionManager::ProjectShapeOnAxis(const std::vector<Vector2>* _v, const Vector2& _axis, float& _min, float& _max)
{
    _min = (*_v)[0].Projection(_axis);
    _max = _min;

    for (int i = 1; i < _v->size(); i++)
    {
        float projection = (*_v)[i].Projection(_axis);
        if (projection < _min) _min = projection;
        if (projection > _max) _max = projection;
    }
    return;
}
