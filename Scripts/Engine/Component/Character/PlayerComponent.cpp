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
#include "../../System/Status/PlayerStatusManager.h"
#include "DxLib.h"

PlayerComponent::PlayerComponent()
	: moveSpeed(0.0f)
	, acceleration(0.0f)
	, avoidMoveValue(0.0f)
	, avoidCoolTime(0.0f)
	, staminaHealCoolTime(0.0f)
	, staminaChangePoint(0.0f)
	, resistTimePoint(0.0f)
	, moveDirectionY(0.0f)
	, canAvoid(true)
	, isAvoid(false)
	, hasResolvedInitialGrounding(false)
	, isDead(false)
	, animator(nullptr)
	, status(-1,-1,-1,-1)

	, PLAYER_MODEL_ANGLE_CORRECTION(89.98f)
	, DEFAULT_MOVE_SPEED(1500.0f)
	, ACCELERATION_RATE(800.0f)
	, RUN_ACCELERATION_MAX(2.0f)
	, AVOID_ACCELERATION_MAX(6.0f)
	, AVOID_MOVE_VALUE_MAX(1000.0f)
	, AVOID_COOL_TIME_MAX(1.0f)
	, STAMINA_HEAL_COOL_TIME_MAX(50.0f)
	, STAMINA_RUN_COST(0.3f)
	, STAMINA_AVOID_COST(10.0f)
	, STAMINA_HEAL_VALUE(0.2f)
	, JUMP_POWER(1400.0f)
	, BACK_ACCELERATION(0.5f)
	, HP_DECREASE_RATE(0.2f)
{
}

void PlayerComponent::Start() {
	// プレイヤーの基礎ステータスを受け取る
	status = PlayerStatusManager::GetInstance().GetPlayerStatusData()->base;
}

void PlayerComponent::Update(float deltaTime) {
	GameObject* player = GetOwner();
	// プレイヤーの基礎ステータス
	PlayerStatusValue baseStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData()->base;

	// 初期接地判定
	if (!hasResolvedInitialGrounding) {
		IsGrounding(player);
		hasResolvedInitialGrounding = true;
	}
	moveVec = Vector3::zero;

	// クールタイムが開け次第スタミナ回復
	if (staminaHealCoolTime <= 0) {
		if (status.stamina < baseStatus.stamina) {
			if (staminaChangePoint >= 1) {
				status.stamina += 1;
				staminaChangePoint = 0;
			}
			else {
				staminaChangePoint += STAMINA_HEAL_VALUE;
			}
		}
	}
	else {
		staminaHealCoolTime -= 1;
	}

	// 耐性値を削っていく
	resistTimePoint -= deltaTime;
	if (resistTimePoint <= -1) {
		if (status.resistTime > 0) {
			status.resistTime -= 1;
		}
		// 耐性値がなくなった場合はHPが割合で削れる
		else {
			status.resistTime = 0;
			if (status.HP > 0)
				status.HP -= baseStatus.HP * HP_DECREASE_RATE;
			else
				status.HP = 0;
		}
		resistTimePoint = 0;
	}
	
	// HPがなくなったら死亡
	if (status.HP <= 0) {
		isDead = true;
	}

	// 回避
	PlayerAvoid(player, deltaTime);
	// 回避中は処理しない
	if (!isAvoid) {
		// 速度調節
		SpeedControl(deltaTime);
		// 移動処理
		PlayerMove(player, deltaTime);
	}
	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(player, moveVec);

	auto playerArm = player->GetComponent<ArmActionComponent>();
	// 物を持っていたらウデ変更不可
	if (playerArm->GetLiftObject())
		return;
	// 右クリックでウデアクションをハンドに設定
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
	}

	// ジャンプ
	if (CheckHitKey(KEY_INPUT_SPACE) && gravity->GetGroundingFrag()) {
		gravity->AddFallSpeed(-JUMP_POWER);
	}

	// アニメーション
	animator = player->GetComponent<AnimatorComponent>();
	if (!animator)
		return;
	auto modelRenderer = player->GetComponent<ModelRenderer>();
	if (!modelRenderer)
		return;
	int modelHandle = modelRenderer->GetModelHandle();
	if (modelHandle == -1)
		return;
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);
	animator->LoadIndex(true);

	// モデルの透明度の調整
	auto handArm = player->GetComponent<ArmActionComponent>();
	if (!handArm->GetLiftObject()) {
		MV1SetOpacityRate(modelHandle, 0);
	}
	else {
		MV1SetOpacityRate(modelHandle, 1);
	}

	// アニメーション再生
	if (CheckHitKey(KEY_INPUT_W)) {
		// 前移動
		animator->Play(4, moveSpeed * 0.066f);
	}
	else if (CheckHitKey(KEY_INPUT_S)) {
		// 後ろ移動
		animator->Play(5, moveSpeed * 0.066f);
	}
	else if (CheckHitKey(KEY_INPUT_A)) {
		// 左移動
		animator->Play(6, moveSpeed * 0.066f);
	}
	else if (CheckHitKey(KEY_INPUT_D)) {
		// 右移動
		animator->Play(7, moveSpeed * 0.066f);
	}
	else  {
		// 待機
		animator->Play(3, 1);
	}
}

/*
 *	速度調節
 */
void PlayerComponent::SpeedControl(float deltaTime) {
	// ダッシュ
	if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_W) && status.stamina > 0) {
		// なめらかな加速
		if (acceleration < RUN_ACCELERATION_MAX)
			acceleration += sinf(Deg2Rad * deltaTime * ACCELERATION_RATE);
		else
			acceleration = RUN_ACCELERATION_MAX;

		// スタミナを消費していく
		if (staminaChangePoint <= -1) {
			status.stamina -= 1;
			staminaChangePoint = 0;
		}
		else {
			staminaChangePoint -= STAMINA_RUN_COST;
		}
		staminaHealCoolTime = STAMINA_HEAL_COOL_TIME_MAX;
	}
	else {
		// なめらかな減速
		if (acceleration > 1)
			acceleration -= sinf(Deg2Rad * deltaTime * ACCELERATION_RATE);
		else
			acceleration = 1;
	}
	// 後ろ歩きは少し遅くする
	if (CheckHitKey(KEY_INPUT_S)) {
		acceleration = BACK_ACCELERATION;
	}
	moveSpeed = DEFAULT_MOVE_SPEED * acceleration;
}

/*
 *	回避
 */
void PlayerComponent::PlayerAvoid(GameObject* player, float deltaTime) {
	// 回避開始
	if (CheckHitKey(KEY_INPUT_LCONTROL) && canAvoid && status.stamina > STAMINA_AVOID_COST) {
		canAvoid = false;
		isAvoid = true;
		moveSpeed = DEFAULT_MOVE_SPEED * AVOID_ACCELERATION_MAX;
		// スタミナ消費
		status.stamina -= STAMINA_AVOID_COST;
		staminaHealCoolTime = STAMINA_HEAL_COOL_TIME_MAX;
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

/*
 * 初期接地チェック
 */
void PlayerComponent::IsGrounding(GameObject* other) {
	other->position.y -= 250.0f;
	// 下方向に微小ベクトルを与える
	Vector3 fakeMove = { 0.0f, other->position.y, 0.0f };

	// 通常の衝突判定を流用
	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(other, fakeMove);
}