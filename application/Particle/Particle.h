#pragma once
#include "Vector2.h"
#include "Vector4.h"
#include <random>

// パーティクル（一個分）
class Particle
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フラグゲッター
	/// </summary>
	/// <returns></returns>
	bool IsFinished()const { return isFinished_; }


private:
	// メンバ変数

	// パーティクルの位置
	Vector2 position_ = {};
	
	// 中心座標
	Vector2 center_ = {};

	// パーティクルの色
	Vector4 color_ = { 0.7294118f, 0.345098f, 0.1764706f, 1.0f };

	Vector2 size_ = {};

	// 描画領域の拡大率
	float expandRate_ = 0.0f; // 初期拡大率
	static inline const float maxExpandRate_ = 1.0f; // 最大拡大率
	float expansionSpeed_ = 0.5f; // 拡大スピード

	// 終了フラグ
	bool isFinished_ = false;
	// 存続時間<秒>
	static inline const float kDuration = 2.0f;
	// 経過時間カウンタ
	float counter_ = 0.0f;


};


