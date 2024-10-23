#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include <vector>
#include"Vector2.h"

class TitleScene : public BaseScene
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
	Sprite* Titlesprite_ = new Sprite();
	Sprite* Spacesprite_ = new Sprite();

	int originalWidth_ = 600;
	int originalHeight_ = 250;

	int width_;
	int height_;

	int x1_;
	int x2_;
	int y1_;
	int y2_;

	// 
	static inline const float kDuration = 3.0f;

	// 経過時間カウント
	float counter_ = 0.0f;

	// 増加フラグ
	bool isIncreasing_ = true;

	// uiテクスチャの色
	int uiColor_ = {};

	int red_;
	int green_;
	int blue_;
	int alpha_;

	bool isDebug_ = false;

	// メンバ関数

	// UIの点滅処理
	void UpdateUI();

	// 色を設定
	unsigned int GetColor(int red, int green, int blue, int alpha);

	/// <summary>
	/// 四角形の座標を設定
	/// </summary>
	void InitRectPostion();

	void UpdateRectPostition();

	void SetRectPosition();
};
