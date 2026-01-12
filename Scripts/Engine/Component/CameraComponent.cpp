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
	, yaw(0.0f)
	, pitch(0.0f)

	, CAMERA_ROTATION_MAX_X(1.5f)
	, CAMERA_ROTATION_MIN_X(-1.5f)
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
	if (handArm->GetLiftObject()) {
		// カメラのターゲット位置
		Vector3 targetPos = player->position;
		targetPos.y += PLAYER_HEAD_HEIGHT;
		// カメラの位置をプレイヤーの背後に配置
		camera->position = targetPos - ForwardDir(camera->rotation) * playerDistancePos;
		// カメラの右方向に移動
		Vector3 right = { cosf(camera->rotation.y), 0.0f, -sinf(camera->rotation.y) };
		right = right.Normalized();
		camera->position -= right * mouseMoveValue.x * 20;
		// カメラの制限付き上下移動
		float cameraPosY = camera->position.y;
		cameraPosY += mouseMoveValue.y * 20;
		cameraPosY = Clamp(cameraPosY, targetPos.y - 300, targetPos.y + 300);
		camera->position.y = cameraPosY;
		// 常にプレイヤーの方を向く
		Vector3 dir = Direction(camera->position, targetPos);
		camera->rotation.x = -atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));;
		camera->rotation.y = atan2f(dir.x, dir.z);
		camera->rotation.z = 0;
	}
	else {
		// 移動量を角度に変換
		Vector3 moveRotation = { mouseMoveValue.y, mouseMoveValue.x, 0 };
		// カメラの角度に移動量を加える
		camera->rotation += moveRotation;
		// x軸の角度は制限を掛ける
		camera->rotation.x = std::clamp(camera->rotation.x, CAMERA_ROTATION_MIN_X, CAMERA_ROTATION_MAX_X);
		// カメラの位置をプレイヤーの頭の位置に合わせる
		Vector3 playerHeadPos = player->position;
		playerHeadPos.y += PLAYER_HEAD_HEIGHT;
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
}
