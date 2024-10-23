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

	bool isDebug_ = false;
};
