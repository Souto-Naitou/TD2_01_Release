#pragma once

#include "Draw2D.h"

#include <Rect2.h>
#include "Object/.Base/BaseObject.h"
#include "Collision/Collider/Collider.h"
#include <string>
#include "Collision/CollisionManager/CollisionManager.h"
#include "Object/HPBar/HPBar.h"
#include <Vector4.h>

class NestWall : public BaseObject
{
public: /// 公開メソッド

    NestWall(std::string _ID);
    ~NestWall();

    void                Initialize();
    void                RunSetMask();
    void                Update();
    void                Draw();



    void                OnCollisionTrigger(const Collider* _collider);
    Collider*           GetCollider() { return &collider_; }
    float               GetHPBarWidth() { return pHpBar_->GetWidth(); }
    float               GetHPBarHeight() { return pHpBar_->GetHeight(); }


	void                SetHPBarPos(Vector2 _pos) { pHpBar_->SetPos(_pos); }
	void                SetHPBarRotation(float _rotation) { pHpBar_->SetRotation(_rotation); }
	void                SetHPBarWidth(float _width) { pHpBar_->SetWidth(_width); }
	void                SetHPBarHeight(float _height) { pHpBar_->SetHeight(_height); }
	void                SetHPBarColor(Vector4 _color) { pHpBar_->SetColor(_color); }
    void                SetRect(int _width, int _height, Vector2 _leftTop, size_t _offset);


private: /// 非公開データ
    Rect2               rect_;
    Collider            collider_;
	HPBar*              pHpBar_;
    bool                isDead_ = false;


private: /// 非公開メソッド
    void                DebugWindow();


private: /// 他オブジェクトのデータ
    CollisionManager*   pCollisionManager_  = nullptr;
    Draw2D*             pDraw2D_            = nullptr;
};