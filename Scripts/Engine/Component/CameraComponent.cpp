/*
 *	@file	CameraComponent.cpp
 *  @author	Riku
 */

#include "CameraComponent.h"
#include "../Manager/CameraManager.h"
#include "DxLib.h"

CameraComponent::CameraComponent()
	: currentMousePosition(Vector3::zero)
	, previousMousePosition(Vector3::zero)
	, mouseMoveValue(Vector3::zero)
	, sensitivity(0.005f) {}

void CameraComponent::Update(float deltaTime) {
	GameObject* camera = GetOwner();
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();

	// 直前のマウスの位置を更新
	previousMousePosition = currentMousePosition;
	// マウスの位置を取得
	int mouseX = 0, mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);
	currentMousePosition.x = mouseX;
	currentMousePosition.y = mouseY;

	if (currentMousePosition == Vector3::zero ||
		previousMousePosition == Vector3::zero) return;
	// 移動量計算
	mouseMoveValue = currentMousePosition - previousMousePosition;
	// 感度を加える
	mouseMoveValue *= sensitivity;
	// 移動量を角度に変換
	Vector3 moveRotation = { mouseMoveValue.y, mouseMoveValue.x, 0 };
	// カメラの角度に移動量を加える
	camera->rotation += moveRotation;
	// カメラの位置をプレイヤーと合わせる
	//if (player != nullptr) 
	//	camera->position = player->position;
	// デバッグ用
	if (CheckHitKey(KEY_INPUT_UP)) {
		camera->position.y += 5;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		camera->position.y -= 5;
	}

	// カメラの設定に反映する
	SetCameraPositionAndAngle(
		Vector3::ToVECTOR(camera->position),
		camera->rotation.x,
		camera->rotation.y,
		moveRotation.z);

}
