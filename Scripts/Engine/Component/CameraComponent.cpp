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

		auto action = GetInputState(GameEnum::ActionMap::PlayerAction);
		// 感度を加える
		float mouseMoveValueX = -action.axis[static_cast<int>(GameEnum::PlayerAction::CameraMoveX)] * sensitivity;
		float mouseMoveValueY = -action.axis[static_cast<int>(GameEnum::PlayerAction::CameraMoveY)] * sensitivity;
		auto handArm = player->GetComponent<ArmActionComponent>();
		// 移動量を角度に変換
		Vector3 moveRotation = { mouseMoveValueY, mouseMoveValueX, 0 };
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
