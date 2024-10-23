#include "GameOverScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Object3dBasic.h"
#include "SpriteBasic.h"
#include "Input.h"
#include "Draw2D.h"

#ifdef _DEBUG
#include"ImGui.h"
#include "DebugCamera.h"
#endif

void GameOverScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	//textureの読み込み
	TextureManager::GetInstance()->LoadTexture("resources/gameover.png");
	TextureManager::GetInstance()->LoadTexture("resources/titleUI_white.png");
	//Sprite初期化
	GameOversprite_->Initialize("resources/gameover.png");
	Spacesprite_->Initialize("resources/titleUI_white.png");
	//位置設定
	GameOversprite_->SetPos({-50,-300 });
	Spacesprite_->SetPos({ 0,-50 });
}

void GameOverScene::Finalize()
{

}

void GameOverScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}

	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	//Spriteの更新
	GameOversprite_->Update();
	Spacesprite_->Update();

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif



	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("title");
	}
}

void GameOverScene::Draw()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	GameOversprite_->Draw();
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

void GameOverScene::DrawImGui()
{
#ifdef _DEBUG


#endif // _DEBUG

}
