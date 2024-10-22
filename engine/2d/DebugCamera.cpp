#include "DebugCamera.h"
#include "WinApp.h"
#include"Input.h"
#include "Vector3.h"

DebugCamera* DebugCamera::instance_ = nullptr;

DebugCamera* DebugCamera::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new DebugCamera();
	}
	return instance_;
}

void DebugCamera::Initialize()
{
	transform_ = { Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -50.0f) };
	fovY_ = 0.45f;
	aspect_ = float(WinApp::kClientWidth) / float(WinApp::kClientHeight);
	nearZ_ = 0.1f;
	farZ_ = 100.0f;
	worldMat_ = Mat4x4::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);
	viewMat_ = Mat4x4::Inverse(worldMat_);
	projectionMat_ = Mat4x4::MakePerspective(fovY_, aspect_, nearZ_, farZ_);
	viewProjectionMat_ = Mat4x4::Multiply(viewMat_, projectionMat_);

	rotMat_ = Mat4x4::MakeRotateXYZ(transform_.rotate);
}

void DebugCamera::Finalize()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void DebugCamera::Update()
{
	if (is2D_)
	{
		Move2D();

		// トランスフォームでワールド行列を作る
		worldMat_ = Mat4x4::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);

		// ビュー行列を作る
		viewMat_ = Mat4x4::Inverse(worldMat_);

		// プロジェクション行列を作る
		projectionMat_ = Mat4x4::MakePerspective(fovY_, aspect_, nearZ_, farZ_);

		// ビュープロジェクション行列を作る
		viewProjectionMat_ = Mat4x4::Multiply(viewMat_, projectionMat_);
	}
	else if (is3D_)
	{
		Move3D();

		Matrix4x4 transMat = Mat4x4::MakeTranslate(transform_.translate);
		
		//  rotMatとtransMatでワールド行列を作る
		worldMat_ = Mat4x4::Multiply(rotMat_, transMat);

		// ビュー行列を作る
		viewMat_ = Mat4x4::Inverse(worldMat_);

		// プロジェクション行列を作る
		projectionMat_ = Mat4x4::MakePerspective(fovY_, aspect_, nearZ_, farZ_);

		// ビュープロジェクション行列を作る
		viewProjectionMat_ = Mat4x4::Multiply(viewMat_, projectionMat_);
	}

}

void DebugCamera::Move2D()
{
	// カメラの移動
	if (Input::GetInstance()->PushKey(DIK_W))
	{

		Vector3 move = Vector3(0.0f, moveSpeed_, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;

	}

	if (Input::GetInstance()->PushKey(DIK_S))
	{

		Vector3 move = Vector3(0.0f, -moveSpeed_, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_A))
	{

		Vector3 move = Vector3(moveSpeed_, 0.0f, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_D))
	{

		Vector3 move = Vector3(-moveSpeed_, 0.0f, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}
}

void DebugCamera::Move3D()
{
	// カメラの移動
	if (Input::GetInstance()->PushKey(DIK_W))
	{

		Vector3 move = Vector3(0.0f, 0.0f, moveSpeed_);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;

	}

	if (Input::GetInstance()->PushKey(DIK_S))
	{

		Vector3 move = Vector3(0.0f, 0.0f, -moveSpeed_);
		// 移動ベクトルを角度分だけ回転させ
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_A))
	{

		Vector3 move = Vector3(-moveSpeed_, 0.0f, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_D))
	{

		Vector3 move = Vector3(moveSpeed_, 0.0f, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_LSHIFT))
	{

		Vector3 move = Vector3(0.0f, -moveSpeed_, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{

		Vector3 move = Vector3(0.0f, moveSpeed_, 0.0f);
		// 移動ベクトルを角度分だけ回転させる
		move = Mat4x4::TransForm(Mat4x4::MakeRotateXYZ(transform_.rotate), move);

		transform_.translate += move;
	}

	// カメラの回転
	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		transform_.rotate.x += rotateSpeed_;
		Matrix4x4 rotMatDelta = Mat4x4::MakeIdentity();
		rotMatDelta *= Mat4x4::MakeRotateX(transform_.rotate.x);

		rotMat_ = Mat4x4::Multiply(rotMatDelta, rotMat_);
	}

	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		transform_.rotate.x -= rotateSpeed_;
		Matrix4x4 rotMatDelta = Mat4x4::MakeIdentity();
		rotMatDelta *= Mat4x4::MakeRotateX(transform_.rotate.x);

		rotMat_ = Mat4x4::Multiply(rotMatDelta, rotMat_);
	}

	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		transform_.rotate.y += rotateSpeed_;
		Matrix4x4 rotMatDelta = Mat4x4::MakeIdentity();
		rotMatDelta *= Mat4x4::MakeRotateY(transform_.rotate.y);

		rotMat_ = Mat4x4::Multiply(rotMatDelta, rotMat_);
	}

	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		transform_.rotate.y -= rotateSpeed_;
		Matrix4x4 rotMatDelta = Mat4x4::MakeIdentity();
		rotMatDelta *= Mat4x4::MakeRotateY(transform_.rotate.y);

		rotMat_ = Mat4x4::Multiply(rotMatDelta, rotMat_);
	}
}
