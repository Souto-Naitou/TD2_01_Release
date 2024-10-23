#include "ClearScene.h"
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
}
 
void ClearScene::Finalize()
{

}

void ClearScene::Update()
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
	Clearsprite_->Update();
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

void ClearScene::Draw()
{
	/// ================================== ///
	///              描画処理               ///
	/// ================================== ///
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