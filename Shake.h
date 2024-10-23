#pragma once
#include "Vector3.h"

/// <summary>
/// シェイク
/// </summary>
class Shake
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="translation"></param>
	void Initialize(const Vector3& translate);

	/// <summary>
	/// ランダムでシェイクさせる
	/// </summary>
	/// <returns></returns>
	Vector3 ApplyRandomShake();

	/// <summary>
	/// 縦揺れさせる
	/// </summary>
	/// <returns></returns>
	Vector3 ApplyVerticalShake();

	/// <summary>
	/// 横揺れさせる
	/// </summary>
	/// <returns></returns>
	Vector3 ApplyHorizontalShake();

	/// <summary>
	/// 位置を戻す
	/// </summary>
	Vector3 ResetPosition();

	/// <summary>
	/// シェイクに必要な値を設定
	/// </summary>
	void SetValue(const Vector3& translate, float shakeScale);

	/// <summary>
	/// シェイク中か否か
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActicve_; }

private:
	// メンバ変数

	// ローカル座標
	Vector3 translate_ = {};
	// 元のローカル座標
	Vector3 originalTranslate_ = {};


	// シェイクの強度に関するスケール係数
	float shakeScale_ = 0.15f;

	// 次のシェイクの方向を決定するためのフラグ
	bool isPositiveDirection_ = true; // 初期値はプラス
	// シェイクの有効化
	bool isActicve_ = false;

private:
	// メンバ関数

	/// <summary>
	/// シェイクの強度を設定
	/// </summary>
	void SetShakeScale();

	/// <summary>
	/// プラスとマイナスを入れ替える
	/// </summary>
	/// <param name="offsetX"></param>
	/// <param name="offsetY"></param>
	void ToggleShakeDirection(float& offsetX, float& offsetY);
};

