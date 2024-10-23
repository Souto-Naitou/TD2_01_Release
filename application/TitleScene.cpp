#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Audio.h"
#include "Draw2D.h"
#include "PostEffect.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void TitleScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	// サウンドの読み込み
	bgmSH_ = Audio::GetInstance()->LoadWaveFile("bgm/titleBGM.wav");
	selectSeSH_ = Audio::GetInstance()->LoadWaveFile("title_space.wav");

	// bgm再生
	bgmVH_ = Audio::GetInstance()->PlayWave(bgmSH_, true, 0.3f);

	PostEffect::GetInstance()->SetBloomThreshold(0.35f);
	PostEffect::GetInstance()->SetVignettePower(0.f);

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

	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///



	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif



	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
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
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();



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
