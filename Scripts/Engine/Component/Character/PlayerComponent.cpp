/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "PlayerComponent.h"
#include "../../Manager/CameraManager.h"
#include "../../GameConst.h"
#include "HandArm.h"
#include "RevolverArm.h"
#include "../../Manager/StageManager.h"
#include "../GravityComponent.h"
#include "DxLib.h"

PlayerComponent::PlayerComponent()
	: moveSpeed(0.0f)
	, acceleration(0.0f)
	, avoidMoveValue(0.0f)
	, avoidCoolTime(0.0f)
	, moveDirectionY(0.0f)
	, canAvoid(true)
	, isAvoid(false)

	, PLAYER_MODEL_ANGLE_CORRECTION(89.98f)
	, DEFAULT_MOVE_SPEED(1500.0f)
	, ACCELERATION_RATE(800.0f)
	, RUN_ACCELERATION_MAX(2.0f)
	, AVOID_ACCELERATION_MAX(6.0f)
	, AVOID_MOVE_VALUE_MAX(1000.0f)
	, AVOID_COOL_TIME_MAX(1.0f) {
}

void PlayerComponent::Update(float deltaTime) {
	GameObject* player = GetOwner();
	moveVec = Vector3::zero;
	// 回避
	PlayerAvoid(player, deltaTime);
	// 回避中は処理しない
	if (isAvoid) return;
	// ダッシュ
	PlayerRun(deltaTime);
	// 移動処理
	PlayerMove(player, deltaTime);
	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(player, moveVec);

	// 右クリックでウデアクションをハンドに設定
	auto playerArm = player->GetComponent<ArmActionComponent>();
	if (GetMouseInput() & MOUSE_INPUT_RIGHT)
		playerArm->SetCurrentArm<HandArm>();
	// 左クリックでウデアクションを銃に設定
	if (GetMouseInput() & MOUSE_INPUT_LEFT)
		playerArm->SetCurrentArm<RevolverArm>();
}

/*
 *	プレイヤーの移動
 */
void PlayerComponent::PlayerMove(GameObject* player, float deltaTime) {
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	// 重力コンポーネントの取得by oorui
	auto gravity = player->GetComponent<GravityComponent>();
	// カメラの角度のsin,cos
	const float cameraSin = sinf(camera->rotation.y);
	const float cameraCos = cosf(camera->rotation.y);

	float moveX = 0;
	float moveZ = 0;

	// カメラから見て左の移動
	if (CheckHitKey(KEY_INPUT_A)) {
		moveX -= moveSpeed * cameraCos * deltaTime;
		moveZ -= moveSpeed * -cameraSin * deltaTime;
	}
	// カメラから見て右の移動
	if (CheckHitKey(KEY_INPUT_D)) {
		moveX += moveSpeed * cameraCos * deltaTime;
		moveZ += moveSpeed * -cameraSin * deltaTime;
	}
	// カメラから見て後ろの移動
	if (CheckHitKey(KEY_INPUT_S)) {
		moveX -= moveSpeed * cameraSin * deltaTime;
		moveZ -= moveSpeed * cameraCos * deltaTime;
	}
	// カメラから見て前の移動
	if (CheckHitKey(KEY_INPUT_W)) {
		moveX += moveSpeed * cameraSin * deltaTime;
		moveZ += moveSpeed * cameraCos * deltaTime;
	}
	player->position.x += moveX;
	player->position.z += moveZ;
	moveVec = { moveX, 0.0f, moveZ };
	// 移動方向を保存
	if (moveX && moveZ)
		moveDirectionY = atan2f(moveX, moveZ);
	// プレイヤーの向きはカメラに合わせる
	player->rotation.y = camera->rotation.y;
	// 角度を補正
	player->rotation.y += PLAYER_MODEL_ANGLE_CORRECTION * Deg2Rad;

	// デバッグ用Y軸移動
	{
		if (CheckHitKey(KEY_INPUT_TAB)) {
			player->position.y -= moveSpeed * deltaTime;
		}
		if (CheckHitKey(KEY_INPUT_SPACE) && gravity->GetGroundingFrag()) {
			gravity->AddFallSpeed(-1400);
		}
	}
}

/*
 *	ダッシュ
 */
void PlayerComponent::PlayerRun(float deltaTime) {
	// なめらかに加速する処理
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		// 加速
		if (acceleration < RUN_ACCELERATION_MAX)
			acceleration += sinf(Deg2Rad * deltaTime * ACCELERATION_RATE);
		else
			acceleration = RUN_ACCELERATION_MAX;
	}
	else {
		// 減速
		if (acceleration > 1)
			acceleration -= sinf(Deg2Rad * deltaTime * ACCELERATION_RATE);
		else
			acceleration = 1;
	}
	moveSpeed = DEFAULT_MOVE_SPEED * acceleration;
}

/*
 *	回避
 */
void PlayerComponent::PlayerAvoid(GameObject* player, float deltaTime) {
	// 回避開始
	if (CheckHitKey(KEY_INPUT_LCONTROL) && canAvoid) {
		canAvoid = false;
		isAvoid = true;
		moveSpeed = DEFAULT_MOVE_SPEED * AVOID_ACCELERATION_MAX;
	}
	if (isAvoid) {
		// プレイヤーの角度のsin,cos
		const float playerSin = sinf(moveDirectionY);
		const float playerCos = cosf(moveDirectionY);
		// プレイヤーの向いている方向に移動
		player->position.x += moveSpeed * playerSin * deltaTime;
		player->position.z += moveSpeed * playerCos * deltaTime;
		// 移動量を加算
		avoidMoveValue += moveSpeed * deltaTime;
		// 特定の距離動いたら回避終了
		if (AVOID_MOVE_VALUE_MAX < avoidMoveValue) {
			avoidMoveValue = 0;
			isAvoid = false;
			avoidCoolTime = AVOID_COOL_TIME_MAX;
		}
	}
	else {
		// クールタイム
		if (avoidCoolTime <= 0) {
			canAvoid = true;
			avoidCoolTime = 0;
		}
		else {
			avoidCoolTime -= deltaTime;
		}
	}
}