#include "NestWall.h"
#include "Externals/ImGuiDebugManager/DebugManager.h"
#include "Helper/ImGuiTemplates/ImGuiTemplates.h"
#include "Object/Enemy/Enemy.h"
#include "Audio.h"

NestWall::NestWall(std::string _ID)
{
    objectID_ = _ID;
    DebugManager::GetInstance()->SetComponent("NestWall", _ID.c_str(), std::bind(&NestWall::DebugWindow, this));

    pCollisionManager_  = CollisionManager::GetInstance();
    pDraw2D_            = Draw2D::GetInstance();
}

NestWall::~NestWall()
{
    if (isDead_) Audio::GetInstance()->PlayWave(deadSH_, false, 0.3f);
    DebugManager::GetInstance()->DeleteComponent(objectID_.c_str());
    pCollisionManager_->DeleteCollider(&collider_);
}

void NestWall::Initialize()
{
    // 仮HP
    hp_ = 50;
    collider_.SetColliderID("NestWall");
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("NestWall"));
    pCollisionManager_->RegisterCollider(&collider_);
	// HPバーの初期化
	pHpBar_ = new HPBar();
	Vector4 color = Vector4(0.572549f, 0.1019608f, 0.25098f, 1.0f);
	pHpBar_->Init(static_cast<float>(hp_), position_, 200.0f, 10.0f, 0.0f, color);

    // コライダーにOnCollisionの関数ポインタを渡す
    collider_.SetOnCollisionTrigger(std::bind(&NestWall::OnCollisionTrigger, this, std::placeholders::_1));
    collider_.SetOwner(this);

	deadSH_ = Audio::GetInstance()->LoadWaveFile("wallBreak.wav");
}

void NestWall::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask("NestWall", "Player", "Core"));
}

void NestWall::Update()
{
    if (hp_ == 0) isDead_ = true;

	pHpBar_->Update(static_cast<float>(hp_));
}

void NestWall::Draw()
{
    Vector4 color = Vector4(0.7215686f, 0.0f, 0.1215686f, 1.0f);
	//Vector4 color = Vector4(0.7294118f, 0.1607843f, 0.2823529f, 1.0f);
    if (isDead_) return;
    pDraw2D_->DrawBox(position_, Vector2(rect_.x2, rect_.y2), 0.0f, color);

	pHpBar_->Draw();
}

void NestWall::OnCollisionTrigger(const Collider* _collider)
{
    if (_collider->GetColliderID() == "Enemy")
    {
        const Enemy* pEnemy = static_cast<const Enemy*>(_collider->GetOwner());
        if (pEnemy->GetIsBounce())
        {
            hp_--;
        }
    }

    if (hp_ < 0) hp_ = 0;
}

void NestWall::SetRect(int _width, int _height, Vector2 _leftTop, size_t _offset = 0)
{
    rect_.MakeRectangle(_width, _height, false);
    Rect2 colliderRect = rect_ + _leftTop;

    std::array<Vector2, 4> vertices;
    vertices[(_offset + 0) % 4] = colliderRect.LeftTop();
    vertices[(_offset + 1) % 4] = colliderRect.RightTop();
    vertices[(_offset + 2) % 4] = colliderRect.RightBottom();
    vertices[(_offset + 3) % 4] = colliderRect.LeftBottom();

    collider_.SetVertices(vertices.data(), 4);
    position_ = _leftTop;
    return;
}

void NestWall::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("id_", objectID_);
        ImGuiTemplate::VariableTableRow("hp_", hp_);
        ImGuiTemplate::VariableTableRow("rect_.LeftTop", rect_.LeftTop());
        ImGuiTemplate::VariableTableRow("rect_.RightBottom", rect_.RightBottom());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionAttribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionMask", collider_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable(objectID_.c_str(), pFunc);
}
