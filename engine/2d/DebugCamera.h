#pragma once
#include "Mat4x4Func.h"

class DebugCamera
{
private: // シングルトン設定

	// インスタンス
	static DebugCamera* instance_;

	DebugCamera() = default;
	~DebugCamera() = default;
	DebugCamera(DebugCamera&) = delete;
	DebugCamera& operator=(DebugCamera&) = delete;

public: // メンバー関数

	///<summary>
	/// インスタンスの取得
	///	</summary>
	static DebugCamera* GetInstance();

	///<summary>
	/// 初期化
	/// </summary>
	void Initialize();

	///<summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	///<summary>
	/// カメラの更新
	/// </summary>
	void Update();

	///<summary>
	/// 2Dカメラの移動操作
	/// </summary>
	void Move2D();

	///<summary>
	/// 3Dカメラの移動操作
	/// </summary>
	void Move3D();

	///<summary>
	/// カメラの2d,3d設定
	/// </summary>
	void Set2D() { 
		is2D_ = true;
		is3D_ = false;
	}
	void Set3D() {
		is3D_ = true;
		is2D_ = false;
	}


	//-----------------------------------------Getter-----------------------------------------//
	///<summary>
	///ビュー行列の取得
	/// </summary>
	Matrix4x4 GetViewMat() const { return viewMat_; }

	///<summary>
	///射影行列の取得
	/// </summary>
	Matrix4x4 GetProjectionMat() const { return projectionMat_; }

	///<summary>
	///ビュー射影行列の取得
	/// </summary>
	Matrix4x4 GetViewProjectionMat() const { return viewProjectionMat_; }

	//-----------------------------------------Setter-----------------------------------------//
	///<summary>
	///ビュー行列の設定
	/// </summary>
	void SetViewMat(const Matrix4x4& viewMatrix) { viewMat_ = viewMatrix; }

	///<summary>
	///射影行列の設定
	/// </summary>
	void SetProjectionMat(const Matrix4x4& projectionMatrix) { projectionMat_ = projectionMatrix; }

	///<summary>
	///ビュー射影行列の設定
	/// </summary>
	void SetViewProjectionMat(const Matrix4x4& viewProjectionMatrix) { viewProjectionMat_ = viewProjectionMatrix; }

	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspect(float aspect) { aspect_ = aspect; }
	void SetNearClip(float nearZ) { nearZ_ = nearZ; }
	void SetFarClip(float farZ) { farZ_ = farZ; }

private: // メンバー変数

	// トランスフォーム
	Transformer transform_;

	// ワールド行列
	Matrix4x4 worldMat_;

	// 回転行列
	Matrix4x4 rotMat_;

	// ビュー行列
	Matrix4x4 viewMat_;

	// プロジェクション行列
	Matrix4x4 projectionMat_;
	float fovY_;
	float aspect_;
	float nearZ_;
	float farZ_;

	// ビュープロジェクション行列
	Matrix4x4 viewProjectionMat_;

	// カメラの移動速度
	float moveSpeed_ = 1.0f;

	// カメラの回転速度
	float rotateSpeed_ = 0.01f;

	// 2Dカメラかどうか
	bool is2D_ = false;

	// 3Dカメラかどうか
	bool is3D_ = true;

};