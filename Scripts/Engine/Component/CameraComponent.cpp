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
	, state(GameEnum::CameraState::FPS)

	, CAMERA_ROTATION_X_MAX(1.5f)
	, CAMERA_ROTATION_X_MIN(-1.5f)
	, PLAYER_HEAD_HEIGHT(310)
{}

/*
 *	最初のUpdateの直前に呼び出される処理
 */
void CameraComponent::Start() {
	// イベントカメラの初期化
	eventCamera = std::make_shared<EventCamera>();
}

/*
 *	更新処理
 */
void CameraComponent::Update(float deltaTime) {
	GameObject* camera = GetOwner();

	// カメラの状態によって更新
	switch (state) {
	case GameEnum::CameraState::FPS:
	case GameEnum::CameraState::TPS:
	{
		GameObjectPtr player = CameraManager::GetInstance().GetTarget();
		if (!player) break;

		// マウスの位置を取得
		int mouseX = 0, mouseY = 0;
		GetMousePoint(&mouseX, &mouseY);
		mousePosition.x = mouseX;
		mousePosition.y = mouseY;

		if (mousePosition == Vector3::zero) break;
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
		if (state == GameEnum::CameraState::TPS) {
			// 物を持っていたらプレイヤーの背後
			camera->position = playerHeadPos - ForwardDir(camera->rotation) * playerDistancePos;
		}
		else {
			// 持っていなかったら頭の位置
			camera->position = playerHeadPos;
		}

		// マウスを画面中央に固定
		SetMousePoint(windowWidthCenter, windowHeightCenter);
	}
		break;
	case GameEnum::CameraState::Event:
		eventCamera->Update(camera, deltaTime);
		break;
	}

	// カメラの設定に反映する
	SetCameraPositionAndAngle(
		Vector3::ToVECTOR(camera->position),
		camera->rotation.x,
		camera->rotation.y,
		camera->rotation.z);
}
