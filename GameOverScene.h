#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include <vector>
#include"Vector2.h"

class GameOverScene : public BaseScene
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui() override;

private: // メンバ変数
	//Sprite作成
	Sprite* GameOversprite_ = new Sprite();
	Sprite* Spacesprite_ = new Sprite();

	static inline const float kDuration = 3.0f;

	// 経過時間カウント
	float counter_ = 0.0f;

	// uiテクスチャの色
	int uiColor_ = {};

	int red_;
	int green_;
	int blue_;
	int alpha_;

	bool isDebug_ = false;

	// UIの点滅処理
	void UpdateUI();

	// 色を設定
	unsigned int GetColor(int red, int green, int blue, int alpha);
};
