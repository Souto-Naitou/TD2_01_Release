#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Audio.h"
#include "Draw2D.h"
#include "PostEffect.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include "Helper/DefaultSettings.h"

#define PI 3.14159265f

#ifdef DEBUG
#include"ImGui.h"
#endif // DEBUG

#include "DebugCamera.h"

float easeOutBounce(float t) {
	if (t < 1 / 2.75f) {
		return 7.5625f * t * t;
	}
	else if (t < 2 / 2.75f) {
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	else if (t < 2.5 / 2.75) {
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	else {
		t -= 2.625f / 2.75f;
		return 7.5625f * t * t + 0.984375f;
	}
}

void TitleScene::Initialize()
{

	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();


	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	// サウンドの読み込み
	bgmSH_ = Audio::GetInstance()->LoadWaveFile("bgm/titleBGM.wav");
	selectSeSH_ = Audio::GetInstance()->LoadWaveFile("title_space.wav");
	//textureの読み込み
	TextureManager::GetInstance()->LoadTexture("resources/titleBarTex.png");
	TextureManager::GetInstance()->LoadTexture("resources/titleUI_white.png");
	//Sprite初期化
	Titlesprite_->Initialize("resources/titleBarTex.png");
	Spacesprite_->Initialize("resources/titleUI_white.png");
	//位置設定
	Titlesprite_->SetPos({ -50,-300 });
	Spacesprite_->SetPos({ 0,-50 });

	// 矩形の初期サイズを設定
	Titlesprite_->SetSize({ originalWidth_,originalHeight_ });
	// bgm再生
	bgmVH_ = Audio::GetInstance()->PlayWave(bgmSH_, true, 0.3f);

	// 色を設定
	red_ = 0xFF;
	green_ = 0xFF;
	blue_ = 0xFF;
	alpha_ = 0xFF;

	unsigned int offset = 20;

	vertices_.push_back(Vector2(offset, offset));
	vertices_.push_back(Vector2(DefaultSettings::kScreenWidth - offset, offset));
	vertices_.push_back(Vector2(DefaultSettings::kScreenWidth - offset, DefaultSettings::kScreenHeight - offset));
	vertices_.push_back(Vector2(offset, DefaultSettings::kScreenHeight - offset));

	/// 回転板の初期化
	pRotateBoard_ = new RotateBoard();
	pRotateBoard_->Initialize();
	pRotateBoard_->SetVertices(&vertices_);
	pRotateBoard_->SetPadding(-1);
}

void TitleScene::Finalize()
{
	// bgm再生停止
	Audio::GetInstance()->StopWave(bgmVH_);

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}
#endif

	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	pRotateBoard_->Update();

	//Spriteの更新
	Titlesprite_->Update();
	Spacesprite_->Update();

	UpdateUI();

	UpdateRectPostition();

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		Audio::GetInstance()->PlayWave(selectSeSH_, false, 0.3f);
		SceneManager::GetInstance()->ChangeScene("game");
	}
}

void TitleScene::Draw()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///

	// 背景
	Draw2D::GetInstance()->DrawBox(
		Vector2(DefaultSettings::kGameScenePosX, DefaultSettings::kGameScenePosY),
		Vector2(DefaultSettings::kGameScreenWidth, DefaultSettings::kGameScreenHeight),
		0.0f, Vector4(0.01f, 0.01f, 0.01f, 1.0f)
	);

	pRotateBoard_->Draw();

	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	Titlesprite_->Draw();
	Spacesprite_->Draw();

	//--------------------------------------------------//


	//-------------------Modelの描画-------------------//
	// 3Dモデル共通描画設定
	Object3dBasic::GetInstance()->SetCommonRenderSetting();



	//------------------------------------------------//


	//------------------前景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();



	//--------------------------------------------------//
}

void TitleScene::DrawImGui()
{
#ifdef _DEBUG


#endif // _DEBUG

}

void TitleScene::UpdateUI()
{
	// カウンターをフレームごとに増加させる（60FPSを想定）
	counter_ += 1.0f / 60.0f;

	// 正弦波を使用して、アルファ値を滑らかに変化させる
	// // 0から1の間を変動させる
	float sineValue = (std::sin(counter_ * 2.0f * PI / kDuration) + 1.0f) / 2.0f;

	// アルファ値を128～255の範囲に
	int minAlpha = 128; // 最小の半透明値
	int maxAlpha = 255; // 最大の不透明値
	alpha_ = static_cast<int>(std::clamp(sineValue * (maxAlpha - minAlpha) + minAlpha, 0.0f, 255.0f));

	uiColor_ = GetColor(red_, green_, blue_, alpha_);

	// アルファ値を含む色を設定
	Spacesprite_->SetColor({
	   static_cast<float>((uiColor_ >> 24) & 0xFF) / 255.0f, // Red
	   static_cast<float>((uiColor_ >> 16) & 0xFF) / 255.0f, // Green
	   static_cast<float>((uiColor_ >> 8) & 0xFF) / 255.0f,  // Blue
	   static_cast<float>(uiColor_ & 0xFF) / 255.0f          // Alpha
	});
}

unsigned int TitleScene::GetColor(int red, int green, int blue, int alpha)
{
	return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

void TitleScene::UpdateRectPostition()
{
	// 元のサイズと縮小と拡大の段階を設定<秒>
	// 元のサイズを保持する時間
	const float holdDuration = 3.0f;
	// 縮小の持続時間
	const float shrinkDuration = 8.0f;
	// 拡大の持続時間
	const float expandDuration = 5.0f;

	// カウンターを進行度に変換（0から1の間）
	float totalDuration = holdDuration + shrinkDuration + expandDuration;
	float t = counter_ / totalDuration;

	// アニメーションがループするようにリセット
	if (t >= 1.0f) {
		// カウンターをリセット
		counter_ = 0.0f;
		// 進行度もリセット
		t = 0.0f;
	}

	// スケールの変化を管理
	float scale = 1.0f;

	if (t < (holdDuration / totalDuration)) {
		// 元のサイズを保持する段階 (0からholdDurationの間)
		// 何もしないので scale は 1.0f のまま
	}
	else if (t < (holdDuration + shrinkDuration) / totalDuration) {
		// 縮小段階 (holdDurationからshrinkDurationの間)
		// 進行度を0から1に正規化
		float shrinkT = (t - (holdDuration / totalDuration)) / (shrinkDuration / totalDuration);
		// 1から0に変化
		scale = 1.0f - easeOutBounce(shrinkT);
	}
	else {
		// 拡大段階 (shrinkDurationからtotalDurationの間)
		// 進行度を0から1に正規化
		float expandT = (t - (holdDuration + shrinkDuration) / totalDuration) / (expandDuration / totalDuration);
		// 0から1に変化
		scale = easeOutBounce(expandT);
	}

	Titlesprite_->SetSize({ originalWidth_ * scale, originalHeight_ * scale });

	float centerX = 600;
	float centerY = 400;
	Titlesprite_->SetPos({ centerX - (originalWidth_ * scale) / 2, centerY - (originalHeight_ * scale) / 2 - 200 });
}

void TitleScene::SetRectPosition()
{

}