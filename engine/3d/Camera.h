#pragma once
#include "Mat4x4Func.h"
#include "Shake.h"

class Camera
{
public: // メンバー関数

	///<summary>
	///コンストラクタ
	/// </summary>
	Camera();

	///<summary>
	///更新
	/// </summary>
	void Update();


	//-----------------------------------------Getter-----------------------------------------//
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	const Vector3& GetRotate() const { return transform_.rotate; }
	const Vector3& GetTranslate() const { return transform_.translate; }


	//-----------------------------------------Setter-----------------------------------------//
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspect(float aspect) { aspect_ = aspect; }
	void SetNearClip(float nearZ) { nearZ_ = nearZ; }
	void SetFarClip(float farZ) { farZ_ = farZ; }
	void SetViewProjectionMatrix(const Matrix4x4& viewProjectionMatrix) { viewProjectionMatrix_ = viewProjectionMatrix; }

private: // メンバー変数

	Shake* pShake_ = nullptr;

	// シェイクフラグ
	bool isShakeing_ = false;
	// 縦揺れフラグ
	bool isVertical_ = false;
	// 横揺れフラグ
	bool isHorizontal_ = false;
	// 元の位置に戻っているかどうか
	bool isReturning_ = false;

	// トランスフォーム
	Transformer transform_;

	// ワールド行列
	Matrix4x4 worldMatrix_;

	// ビュー行列
	Matrix4x4 viewMatrix_;

	// プロジェクション行列
	Matrix4x4 projectionMatrix_;
	float fovY_;
	float aspect_;
	float nearZ_;
	float farZ_;

	// ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;
};