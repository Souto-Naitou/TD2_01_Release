#include "ClearScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"
#include <algorithm>
#include <cmath>
#include "PostEffect.h"

#define PI 3.14159265f

#ifdef _DEBUG
#include"ImGui.h"
#endif
#include "DebugCamera.h"

void ClearScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	//textureの読み込み
	TextureManager::GetInstance()->LoadTexture("resources/gameclear.png");
	TextureManager::GetInstance()->LoadTexture("resources/titleUI_white.png");
	//Sprite初期化
	Clearsprite_->Initialize("resources/gameclear.png");
	Spacesprite_->Initialize("resources/titleUI_white.png");
	//位置設定
	Clearsprite_->SetPos({ -50,-300 });
	Spacesprite_->SetPos({ 0,-50 });

	// 色を設定
	red_ = 0xFF;
	green_ = 0xFF;
	blue_ = 0xFF;
	alpha_ = 0xFF;

	PostEffect::GetInstance()->SetBloomThreshold(1.f);
	PostEffect::GetInstance()->SetVignettePower(0.f);
}

void ClearScene::Finalize()
{
	delete Clearsprite_;
	delete Spacesprite_;
}

void ClearScene::Update()
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

	//Sprite更新
	Clearsprite_->Update();
	Spacesprite_->Update();

	UpdateUI();

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}



	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("title");
	}
}

void ClearScene::Draw()
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

	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	Clearsprite_->Draw();
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

void ClearScene::DrawImGui()
{
#ifdef _DEBUG


#endif // _DEBUG

}


void ClearScene::UpdateUI() {// カウンターをフレームごとに増加させる（60FPSを想定）
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

unsigned int ClearScene::GetColor(int red, int green, int blue, int alpha)
{
	return (red << 24) | (green << 16) | (blue << 8) | alpha;
}