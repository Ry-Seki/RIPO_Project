/*
 *	@file	CameraComponent.cpp
 *  @author	Riku
 */

#include "CameraComponent.h"
#include "../Manager/CameraManager.h"
#include "../GameConst.h"
#include "Character/ArmActionComponent.h"
#include "DxLib.h"
#include <algorithm>

CameraComponent::CameraComponent()
	: mousePosition(Vector3::zero)
	, mouseMoveValue(Vector3::zero)
	, sensitivity(0.005f)
	, playerDistancePos(500)

	, CAMERA_ROTATION_X_MAX(1.5f)
	, CAMERA_ROTATION_X_MIN(-1.5f)
	, PLAYER_HEAD_HEIGHT(310)
{}

void CameraComponent::Update(float deltaTime) {
	GameObject* camera = GetOwner();
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	if (!player) return;

	// マウスの位置を取得
	int mouseX = 0, mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);
	mousePosition.x = mouseX;
	mousePosition.y = mouseY;

	if (mousePosition == Vector3::zero) return;
	// 画面中央
	int windowWidthCenter = GameConst::WINDOW_WIDTH / 2;
	int windowHeightCenter = GameConst::WINDOW_HEIGHT / 2;

	// 移動量計算
	mouseMoveValue.x = mousePosition.x - windowWidthCenter;
	mouseMoveValue.y = mousePosition.y - windowHeightCenter;
	// 感度を加える
	mouseMoveValue *= sensitivity;
	auto handArm = player->GetComponent<ArmActionComponent>();
	// 移動量を角度に変換
	Vector3 moveRotation = { mouseMoveValue.y, mouseMoveValue.x, 0 };
	// カメラの角度に移動量を加える
	camera->rotation += moveRotation;
	// x軸の角度は制限を掛ける
	camera->rotation.x = Clamp(camera->rotation.x, CAMERA_ROTATION_X_MIN, CAMERA_ROTATION_X_MAX);
	// カメラの位置調整
	Vector3 playerHeadPos = player->position;
	playerHeadPos.y += PLAYER_HEAD_HEIGHT;
	if (handArm->GetLiftObject()) {
		// 物を持っていたらプレイヤーの背後
		camera->position = playerHeadPos - ForwardDir(camera->rotation) * playerDistancePos;
	}
	else {
		// 持っていなかったら頭の位置
		camera->position = playerHeadPos;
	}

#if _DEBUG
	// デバッグ用移動
	{
		const float cameraCos = cosf(camera->rotation.y);
		const float cameraSin = sinf(camera->rotation.y);
		float moveSpeed = 50;
		if (CheckHitKey(KEY_INPUT_RSHIFT)) {
			camera->position.y += moveSpeed;
		}
		if (CheckHitKey(KEY_INPUT_RCONTROL)) {
			camera->position.y -= moveSpeed;
		}
		// カメラから見たX軸移動
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			camera->position.x -= moveSpeed * cameraCos;
			camera->position.z -= moveSpeed * -cameraSin;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT)) {
			camera->position.x += moveSpeed * cameraCos;
			camera->position.z += moveSpeed * -cameraSin;
		}
		// カメラから見たZ軸移動
		if (CheckHitKey(KEY_INPUT_DOWN)) {
			camera->position.x -= moveSpeed * cameraSin;
			camera->position.z -= moveSpeed * cameraCos;
		}
		if (CheckHitKey(KEY_INPUT_UP)) {
			camera->position.x += moveSpeed * cameraSin;
			camera->position.z += moveSpeed * cameraCos;
		}
	}
#endif

	// マウスを画面中央に固定
	SetMousePoint(windowWidthCenter, windowHeightCenter);
	// カメラの設定に反映する
	SetCameraPositionAndAngle(
		Vector3::ToVECTOR(camera->position),
		camera->rotation.x,
		camera->rotation.y,
		camera->rotation.z);

	// スクリーンの中心設定
	SetCameraScreenCenter(GameConst::WINDOW_WIDTH * 0.5f, GameConst::WINDOW_HEIGHT * 0.5f);
}
