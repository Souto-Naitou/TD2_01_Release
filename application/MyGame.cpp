#include "MyGame.h"
#include"Audio.h"
#include"Input.h"
#include "SceneFactory.h"
#include "SceneManager.h"
#include "Draw2D.h"
#include "Object3dBasic.h"
#include "PostEffect.h"
#include "externals/ImGuiDebugManager/DebugManager.h"
#include "externals/easing/EasingManager/EasingManager.h"

void MyGame::Initialize()
{

	TakoFramework::Initialize();

#pragma region 汎用機能初期化-------------------------------------------------------------------------------------------------------------------
	// 入力クラスの初期化
	Input::GetInstance()->Initialize(winApp_);

	// オーディオの初期化
	Audio::GetInstance()->Initialize("resources/Sound/");

#pragma endregion

	// シーンの初期化
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("title", 0.0f);
    DebugManager::GetInstance()->ChangeFont();
}

void MyGame::Finalize()
{
	// Audioの解放
	Audio::GetInstance()->Finalize();

	// 入力クラスの解放
	Input::GetInstance()->Finalize();

	TakoFramework::Finalize();
}

void MyGame::Update()
{

	TakoFramework::Update();

	// 入力情報の更新
	Input::GetInstance()->Update();

	//　サウンドの更新
	Audio::GetInstance()->Update();

	// カメラの更新
	defaultCamera_->Update();

}

void MyGame::Draw()
{
	/// ============================================= ///
	/// ------------------シーン描画-------------------///
	/// ============================================= ///

	// 描画前の処理(レンダーテクスチャを描画対象に設定)
	dx12_->SetRenderTexture();

	// テクスチャ用のsrvヒープの設定
	SrvManager::GetInstance()->BeginDraw();

	// シーンの描画
	SceneManager::GetInstance()->Draw();

	Draw2D::GetInstance()->Reset();


	/// ===================================================== ///
	/// ------------------ポストエフェクト描画-------------------///
	/// ===================================================== ///
	// SwapChainを描画対象に設定
	dx12_->SetSwapChain();

	// PostEffectの描画
	switch (postEffectType)
	{
	case::MyGame::VignetteRed:
		PostEffect::GetInstance()->Draw("VignetteRed");
		break;
	case::MyGame::VignetteRedBloom:
		PostEffect::GetInstance()->Draw("VignetteRedBloom");
		break;
	case::MyGame::GrayScale:
		PostEffect::GetInstance()->Draw("GrayScale");
		break;
	case::MyGame::VigRedGrayScale:
		PostEffect::GetInstance()->Draw("VigRedGrayScale");
		break;
	}


	/// ========================================= ///
	///-------------------ImGui-------------------///
	/// ========================================= ///
#ifdef _DEBUG

	imguiManager_->Begin();

	SceneManager::GetInstance()->DrawImGui();

	Draw2D::GetInstance()->ImGui();

    DebugManager::GetInstance()->DrawUI();

	EasingManager::GetInstance()->DrawUI();

	// PostEffectのパラメータ調整
	ImGui::Begin("PostEffect");
	if (ImGui::BeginTabBar("PostEffectTab"))
	{

		if (ImGui::BeginTabItem("PostEffectType"))
		{
			ImGui::RadioButton("VignetteRed", (int*)&postEffectType, VignetteRed);
			ImGui::RadioButton("VignetteRedBloom", (int*)&postEffectType, VignetteRedBloom);
			ImGui::RadioButton("GrayScale", (int*)&postEffectType, GrayScale);
			ImGui::RadioButton("VigRedGrayScale", (int*)&postEffectType, VigRedGrayScale);

			ImGui::EndTabItem();
		}

		//ImGui::Separator();
		if (ImGui::BeginTabItem("PostEffect"))
		{
			if (postEffectType == VignetteRed || postEffectType == VignetteRedBloom || postEffectType == VigRedGrayScale)
			{
				ImGui::DragFloat("VignettePower", &vignettePower, 0.01f, 0.0f, 10.0f);
				PostEffect::GetInstance()->SetVignettePower(vignettePower);
			}

			if (postEffectType == VignetteRedBloom)
			{
				ImGui::DragFloat("BloomThreshold", &bloomThreshold, 0.01f, 0.0f, 1.0f);
				PostEffect::GetInstance()->SetBloomThreshold(bloomThreshold);
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();

	imguiManager_->End();

	//imguiの描画
	imguiManager_->Draw();
#endif


	// 描画後の処理
	dx12_->EndDraw();
}
