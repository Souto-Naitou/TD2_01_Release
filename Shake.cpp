#include "Shake.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.141592f

void Shake::Initialize(const Vector3& translate)
{
	translate_ = translate;

	srand(static_cast<unsigned int>(time(0)));
}

Vector3 Shake::ApplyRandomShake()
{
	// シェイクの強度を設定
	SetShakeScale();

	// ランダムな角度を生成（0から360度の範囲）
	float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI; // 0~2πの範囲

	float scaleMultiplier = 5.0f;

	// 角度からX,Yのオフセットを計算
	float offsetX = cosf(angle) * scaleMultiplier * shakeScale_;
	float offsetY = sinf(angle) * scaleMultiplier * shakeScale_;

	// 共通処理でプラスマイナスを切り替える
	ToggleShakeDirection(offsetX, offsetY);

	// 振動を加える
	return Vector3(offsetX, offsetY, 0.0f);
}

Vector3 Shake::ApplyVerticalShake()
{
	// シェイクの強度を設定
	SetShakeScale();

	float scaleMultiplier = 2.0f;

	// ランダムな縦方向の振動を生成
	float offsetY = (rand() % 50 / 50.0f - 0.1f) * scaleMultiplier * shakeScale_;

	// プラスマイナスを切り替える（Y方向のみ）
	float dummy = 0.0f; // 横方向は使わないのでダミー変数
	ToggleShakeDirection(dummy, offsetY);

	// 振動をY軸方向に加える
	return Vector3(0.0f, offsetY, 0.0f);
}

Vector3 Shake::ApplyHorizontalShake()
{
	// シェイクの強度を設定
	SetShakeScale();

	float scaleMultiplier = 2.0f;

	// ランダムな横方向の振動を生成
	float offsetX = (rand() % 50 / 50.0f - 0.1f) * scaleMultiplier * shakeScale_;

	// プラスマイナスを切り替える（X方向のみ）
	float dummy = 0.0f; // 縦方向は使わないのでダミー変数
	ToggleShakeDirection(offsetX, dummy);


	// 振動をX軸方向に加える
	return Vector3(offsetX, 0.0f, 0.0f);
}

Vector3 Shake::ResetPosition()
{
	translate_ = originalTranslate_;
	return translate_;
}

void Shake::SetValue(const Vector3& translation, float shakeScale)
{
	// 時間を設定する
	originalTranslate_ = translation;

	// シェイクの強度を初期設定に
	shakeScale_ = shakeScale;

	// シェイクを有効に
	isActicve_ = true;
}

void Shake::SetShakeScale()
{
	// シェイクの強度が0以上なら
	if (shakeScale_ >= 0.0f) {

		// スケール係数を徐々に減算
		shakeScale_ -= 0.01f;

	} else {

		// シェイクを無効に
		isActicve_ = false;

	}
}

void Shake::ToggleShakeDirection(float& offsetX, float& offsetY)
{
	// プラスとマイナスを切り替える
	if (isPositiveDirection_)
	{
		offsetX = static_cast<float>(fabs(offsetX)); // プラスのオフセット
		offsetY = static_cast<float>(fabs(offsetY)); // プラスのオフセット
	} else
	{
		offsetX = static_cast<float>(-fabs(offsetX)); // マイナスのオフセット
		offsetY = static_cast<float>(-fabs(offsetY)); // マイナスのオフセット
	}

	// 次の方向を反転
	isPositiveDirection_ = !isPositiveDirection_;
}