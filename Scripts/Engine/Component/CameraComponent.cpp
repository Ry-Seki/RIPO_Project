/*
 *	@file	CameraComponent.cpp
 *  @author	Riku
 */

#include "CameraComponent.h"
#include "DxLib.h"

CameraComponent::CameraComponent()
	: currentPosition(Vector3::zero)
	, previousPosition(Vector3::zero)
	, moveValue(Vector3::zero) 
	, sensitivity(0.0f) {
}

void CameraComponent::Update(float deltaTime) {
	GameObject* camera = GetOwner();

	// 直前の位置を更新
	previousPosition = currentPosition;
	// マウスの位置を取得
	int mouseX = 0, mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);
	currentPosition.x = mouseX;
	currentPosition.y = mouseY;
	
	if (currentPosition == Vector3::zero ||
		previousPosition == Vector3::zero) return;
	// 移動量計算
	moveValue = currentPosition - previousPosition;
	// 感度を加える
	moveValue *= sensitivity;
	// 移動量を角度に変換
	Vector3 moveRotation = { moveValue.y, moveValue.x, 0 };
	// カメラの角度に移動量を加える
	camera->rotation += moveRotation;

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Camera.rotation : %.2f, %.2f, %.2f", camera->rotation.x, camera->rotation.y, camera->rotation.z);
}
