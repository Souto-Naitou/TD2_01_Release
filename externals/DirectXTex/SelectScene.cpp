#include "SelectScene.h"
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

void SelectScene::Initialize()
{
#ifdef _DEBUG
	DebugCamera::GetInstance()->Initialize();
	DebugCamera::GetInstance()->Set2D();
#endif

	/// ================================== ///
	///              初期化処理              ///
	/// ================================== ///

	//textureの読み込み
	TextureManager::GetInstance()->LoadTexture("resources/easy.png");
	TextureManager::GetInstance()->LoadTexture("resources/normal.png");
	TextureManager::GetInstance()->LoadTexture("resources/hard.png");
	//Sprite初期化
	Easysprite_->Initialize("resources/easy.png");
	Normalsprite_->Initialize("resources/normal.png");
	Hardsprite_->Initialize("resources/hard.png");

	//位置設定
	Easysprite_->SetPos({ 250,300 });
	Normalsprite_->SetPos({ 550,300 });
	Hardsprite_->SetPos({ 850,300 });

	
}

void SelectScene::Finalize()
{

}

void SelectScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		Draw2D::GetInstance()->SetDebug(!Draw2D::GetInstance()->GetDebug());
		isDebug_ = !isDebug_;
	}

	/// ================================== ///
	///              更新処理               ///
	/// ================================== ///

	//Sprite更新
	Easysprite_->Update();
	Normalsprite_->Update();
	Hardsprite_->Update();

	if (isDebug_) {
		DebugCamera::GetInstance()->Update();
	}
#endif



	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("game");
	}
}

void SelectScene::Draw()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
	//------------------背景Spriteの描画------------------//
	// スプライト共通描画設定
	SpriteBasic::GetInstance()->SetCommonRenderSetting();

	Easysprite_->Draw();
	Normalsprite_->Draw();
	Hardsprite_->Draw();
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

void SelectScene::DrawImGui()
{
#ifdef _DEBUG


#endif // _DEBUG

}
