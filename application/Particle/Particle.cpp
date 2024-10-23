#include "Particle.h"
#include "math.h"
#include <time.h>
#include "Draw2D.h"
#include "DefaultBrowserSyncSettings.h"

#define Width_Difference 100
#define Height_Difference 100

void Particle::Initialize()
{
	// 中心座標を計算
	center_ = { DefaultSettings::kGameScreenWidth / 2.0f,DefaultSettings::kGameScreenHeight / 2.0f };

	// 描画する横幅
	float minWid = center_.x - Width_Difference;
	float maxWid = center_.x + Width_Difference;

	// 縦幅
	float minHigh = center_.y - Height_Difference;
	float maxhigh = center_.y + Height_Difference;

	position_ = { minWid + static_cast<float>(rand()) / RAND_MAX * (maxWid - minWid),
	minHigh + static_cast<float>(rand()) / RAND_MAX * (maxhigh - minHigh) };

	// パーティクルの情報
	size_ = { 5.0f,5.0f };
	isFinished_ = false;
	counter_ = 0.0f;
}

void Particle::Update()
{
	if (isFinished_) {
		return;
	}

	counter_ += 1.0f / 60.0f;

	// 3秒経ったら消える
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了する
		isFinished_ = true;
	}

	//--------------------------------
	// 画面の中心引き寄せられる
	//--------------------------------

	// 位置の差分を計算
	Vector2 direction = center_ - position_;

	// 移動速度を設定
	float speed = 100.0f * (1.0f / 60.0f);

	float length = sqrtf(direction.x * direction.x + direction.y * direction.y);

	if (length > 0.001f) {
		direction.x /= length;
		direction.y /= length;
	}

	position_.x += direction.x * speed;
	position_.y += direction.y * speed;

	//--------------------------------
	// 色を徐々に透明に
	//--------------------------------

	color_.w = std::clamp((kDuration - counter_) / kDuration, 0.0f, 1.0f);

	//--------------------------------
	// パーティクルのサイズを小さくする
	//--------------------------------

	// 雑
	size_.x = size_.y -= 0.1f;
	

}

void Particle::Draw()
{
	if (isFinished_) {
		return;
	}

	// 四角を描画
	Draw2D::GetInstance()->DrawBox(position_, size_, color_);
}
