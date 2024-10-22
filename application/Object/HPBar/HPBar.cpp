#include "HPBar.h"
#include <cmath>
#include <array>

void HPBar::Init(float _maxHP, Vector2 _pos, float _maxHPwidth, float _height, float _rotation, Vector4 _color)
{
    pDraw2D_ = Draw2D::GetInstance();

	hp_ = _maxHP;
	maxHP_ = _maxHP;

	pos_ = _pos;

	maxHPWidth_ = _maxHPwidth;

	height_ = _height;
	width_ = _maxHPwidth;

	rotation_ = _rotation;
	color_ = _color;
}

void HPBar::Update(float _hp)
{
	width_ = (_hp / maxHP_) * maxHPWidth_;
}

void HPBar::Draw()
{
	pDraw2D_->DrawBox(pos_, Vector2(width_, height_) , rotation_, color_);

	// outline
	float offset = 2.0f;

	float left = -offset;
	float top = -offset;
	float right = maxHPWidth_ + offset;
	float bottom = height_ + offset;

	// 頂点データ
	std::array vertices = {
		Vector2{left,  top   }, // 左上
		Vector2{right, top   }, // 右上
		Vector2{left,  bottom}, // 左下
		Vector2{right, bottom}, // 右下
	};

	// 回転
	for (auto& vertex : vertices) {
		// 回転
		vertex = {
			vertex.x * cosf(rotation_) + vertex.y * -sinf(rotation_),
			vertex.x * sinf(rotation_) + vertex.y * cosf(rotation_) };
		// 平行移動
		vertex.x += pos_.x;
		vertex.y += pos_.y;
	}

    Vector4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	pDraw2D_->DrawLine(vertices[0], vertices[1], white);
    pDraw2D_->DrawLine(vertices[0], vertices[2], white);
    pDraw2D_->DrawLine(vertices[1], vertices[3], white);
    pDraw2D_->DrawLine(vertices[2], vertices[3], white);
}


