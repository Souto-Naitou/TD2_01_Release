#pragma once
#include <Windows.h>
#include <cstdint>
#include "Helper/DefaultSettings.h"

class WinApp {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	HWND GetHWnd() const { return hWnd_; }

	/// <summary>
	/// hInstanceの取得
	/// </summary>
	HINSTANCE GetHInstance() const { return wc_.hInstance; }

public:
	//クライアント領域のサイズ
	static const int32_t kClientWidth = DefaultSettings::kGameScreenWidth;
	static const int32_t kClientHeight = DefaultSettings::kGameScreenHeight;

private:
	//ウィンドウハンドル
	HWND hWnd_ = nullptr;

	//ウィンドウクラス
	WNDCLASS wc_{};
};