#include"Camera.h"
#include "WinApp.h"
#include "Input.h"
#include "Draw2D.h"

Camera::Camera() : 
	transform_({ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f) }),
	fovY_(0.45f), 
	aspect_(float(WinApp::kClientWidth) / float(WinApp::kClientHeight)),
	nearZ_(0.1f), 
	farZ_(100.0f), 
	worldMatrix_(Mat4x4::MakeAffine(transform_.scale, transform_.rotate, transform_.translate)),
	viewMatrix_(Mat4x4::Inverse(worldMatrix_)),
	projectionMatrix_(Mat4x4::MakePerspective(fovY_, aspect_, nearZ_, farZ_)),
	viewProjectionMatrix_(Mat4x4::Multiply(viewMatrix_, projectionMatrix_))
{
	pShake_ = new Shake();
	pShake_->Initialize(transform_.translate);
}


void Camera::Update()
{
	// シェイクを開始する
	if (Input::GetInstance()->TriggerKey((DIK_1)) && !isShakeing_) {
		isShakeing_ = true;
		pShake_->SetValue(transform_.translate, 0.5f);
	}

	// 縦揺れを開始する
	if (Input::GetInstance()->TriggerKey((DIK_2)) && !isVertical_) {
		isVertical_ = true;
		pShake_->SetValue(transform_.translate, 0.5f);
	}

	// 横揺れを開始する
	if (Input::GetInstance()->TriggerKey((DIK_3)) && !isHorizontal_) {
		isHorizontal_ = true;
		pShake_->SetValue(transform_.translate, 0.5f);
	}

	// シェイク処理
	if (isShakeing_) {
		if (pShake_->GetIsActive()) {
			Vector3 shakeOffset = pShake_->ApplyRandomShake();
			transform_.translate += shakeOffset;
		} else {
			isShakeing_ = false;
			isReturning_ = true;
		}
	}

	// 縦揺れ処理
	if (isVertical_) {
		if (pShake_->GetIsActive()) {
			Vector3 shakeOffset = pShake_->ApplyVerticalShake();
			transform_.translate += shakeOffset;
		} else {
			isVertical_ = false;
			isReturning_ = true;
		}
	}

	// 横揺れ処理
	if (isHorizontal_) {
		if (pShake_->GetIsActive()) {
			Vector3 shakeOffset = pShake_->ApplyHorizontalShake();
			transform_.translate += shakeOffset;
		} else {
			isHorizontal_ = false;
			isReturning_ = true;
		}
	}

	// シェイクが終わったらカメラの位置を初期位置に戻す
	if (isReturning_) {
		transform_.translate = pShake_->ResetPosition();
		isReturning_ = false;
	}

	// トランスフォームでワールド行列を作る
	worldMatrix_ = Mat4x4::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);

	// ビュー行列を作る
	viewMatrix_ = Mat4x4::Inverse(worldMatrix_);

	// プロジェクション行列を作る
	projectionMatrix_ = Mat4x4::MakePerspective(fovY_, aspect_, nearZ_, farZ_);

	// ビュープロジェクション行列を作る
	//viewProjectionMatrix_ = Mat4x4::Multiply(viewMatrix_, projectionMatrix_);

	// Releseの時のカメラのviewMatrixを設定
	Draw2D::GetInstance()->SetViewMatrix(viewMatrix_);

}
