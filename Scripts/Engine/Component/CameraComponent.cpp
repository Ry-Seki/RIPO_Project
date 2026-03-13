/*
 *	@file	CameraComponent.cpp
 *  @author	Riku
 */

#include "CameraComponent.h"
#include "../Manager/CameraManager.h"
#include "../GameConst.h"
#include "Character/ArmActionComponent.h"
#include "../Input/InputUtility.h"
#include "DxLib.h"
#include <algorithm>

using namespace InputUtility;

CameraComponent::CameraComponent()
	: sensitivity(0.005f)
	, playerDistancePos(500)
	, state(GameEnum::CameraState::FPS)

	, CAMERA_ROTATION_X_MAX(1.5f)
	, CAMERA_ROTATION_X_MIN(-1.5f)
	, PLAYER_HEAD_HEIGHT(310)
	, DEFAULT_DEBUG_MOVE_SPEED(1500.0f) {}

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
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	if (!player) return;

	// カメラの状態によって更新
	switch (state) {
	case GameEnum::CameraState::FPS:
	{
		// カメラの回転
		auto playerAction = GetInputState(GameEnum::ActionMap::PlayerAction);
		float axisX = playerAction.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateX)];
		float axisY = playerAction.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateY)];
		CameraRotate(camera, axisX, axisY);

		// プレイヤーの頭に位置調整
		Vector3 playerHeadPos = player->position;
		playerHeadPos.y += PLAYER_HEAD_HEIGHT;
		camera->position = playerHeadPos;
		break;
	}
	case GameEnum::CameraState::TPS:
	{
		// カメラの回転
		auto playerAction = GetInputState(GameEnum::ActionMap::PlayerAction);
		float axisX = playerAction.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateX)];
		float axisY = playerAction.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateY)];
		CameraRotate(camera, axisX, axisY);

		// プレイヤーの背後に位置調整
		Vector3 playerHeadPos = player->position;
		playerHeadPos.y += PLAYER_HEAD_HEIGHT;
		camera->position = playerHeadPos - ForwardDir(camera->rotation) * playerDistancePos;
		break;
	}
	case GameEnum::CameraState::Event:
		// イベントカメラの更新
		eventCamera->Update(camera, deltaTime);
		break;
	case GameEnum::CameraState::Debug:
	{
		// カメラの回転
		auto cameraAction = GetInputState(GameEnum::ActionMap::DebugCameraAction);
		float axisX = cameraAction.axis[static_cast<int>(GameEnum::DebugCameraAction::RotateX)];
		float axisY = cameraAction.axis[static_cast<int>(GameEnum::DebugCameraAction::RotateY)];
		CameraRotate(camera, axisX, axisY);

		// カメラの角度のsin,cos
		const float cameraSin = sinf(camera->rotation.y);
		const float cameraCos = cosf(camera->rotation.y);

		// 移動
		Vector3 moveVec = V_ZERO;
		float right = cameraAction.axis[static_cast<int>(GameEnum::DebugCameraAction::RightMove)];
		float forward = cameraAction.axis[static_cast<int>(GameEnum::DebugCameraAction::ForwardMove)];
		float up = cameraAction.axis[static_cast<int>(GameEnum::DebugCameraAction::UpMove)];
		// 移動速度
		float moveSpeed = DEFAULT_DEBUG_MOVE_SPEED;
		if (cameraAction.button[static_cast<int>(GameEnum::DebugCameraAction::Run)]) {
			moveSpeed *= 2;
		}

		// カメラから見て右の移動
		if (right != 0) {
			moveVec.x += right * moveSpeed * cameraCos * deltaTime;
			moveVec.z += right * moveSpeed * -cameraSin * deltaTime;
		}
		// カメラから見て前の移動
		if (forward != 0) {
			moveVec.x += forward * moveSpeed * cameraSin * deltaTime;
			moveVec.z += forward * moveSpeed * cameraCos * deltaTime;
		}
		// カメラから見て上の移動
		if (up != 0) {
			moveVec.y += up * moveSpeed * deltaTime;
		}
		// 2方向に入力されていたら移動量半減
		if (fabs(right) + fabs(forward) == 2) {
			moveVec *= 0.5f;
		}
		camera->position += moveVec;

		// プレイヤー操作に切り替え
		if (!player) break;
		if (cameraAction.buttonDown[static_cast<int>(GameEnum::DebugCameraAction::ChangePlayerAction)]) {
			// アクションマップの切り替え
			SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, true);
			SetActionMapIsActive(GameEnum::ActionMap::DebugCameraAction, false);
			// カメラステートの切り替え
			if (!player->GetComponent<ArmActionComponent>()->GetLiftObject()) {
				state = GameEnum::CameraState::FPS;
			}
			else {
				state = GameEnum::CameraState::TPS;
			}
		}
		break;
	}
	}


	// カメラの設定に反映する
	SetCameraPositionAndAngle(
		Vector3::ToVECTOR(camera->position),
		camera->rotation.x,
		camera->rotation.y,
		camera->rotation.z);
}

/*
 *	カメラの回転
 */
void CameraComponent::CameraRotate(GameObject* camera, float axisX, float axisY) {
	// 感度を加える
	float mouseMoveValueX = -axisX * sensitivity;
	float mouseMoveValueY = -axisY * sensitivity;

	// 移動量を角度に変換
	Vector3 moveRotation = { mouseMoveValueY, mouseMoveValueX, 0 };
	// カメラの角度に移動量を加える
	camera->rotation += moveRotation;
	// x軸の角度は制限を掛ける
	camera->rotation.x = Clamp(camera->rotation.x, CAMERA_ROTATION_X_MIN, CAMERA_ROTATION_X_MAX);
}
