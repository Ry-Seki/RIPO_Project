/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "PlayerComponent.h"
#include "../../Manager/CameraManager.h"
#include "../../GameConst.h"
#include "../../Manager/StageManager.h"
#include "../GravityComponent.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../Input/InputUtility.h"
#include "BulletComponent.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Audio/LoadAudio.h"
#include "../../Load/Model/LoadModel.h"
#include "../../Audio/AudioUtility.h"
#include "../../Manager/StageManager.h"
#include "CharacterUtility.h"
#include "DxLib.h"

using namespace InputUtility;
using namespace AudioUtility;
using namespace CharacterUtility;

PlayerComponent::PlayerComponent()
	: moveSpeed(0.0f)
	, acceleration(0.0f)
	, avoidMoveValue(0.0f)
	, avoidCoolTime(0.0f)
	, staminaHealCoolTime(0.0f)
	, staminaChangePoint(0.0f)
	, resistTimePoint(0.0f)
	, resistDownSpeed(0.0f)
	, moveDirectionY(0.0f)
	, walkSECoolTime(0.0f)
	, canAvoid(true)
	, isAvoid(false)
	, hasResolvedInitialGrounding(false)
	, isDead(false)
	, animator(nullptr)
	, status({ -1, -1, -1, -1 })
	, playerModelHandle(-1)

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
	, WORK_SE_COOL_TIME_MAX(33.0f){}

void PlayerComponent::Start() {
	animator = GetOwner()->GetComponent<AnimatorComponent>();
	// プレイヤーの基礎ステータスを受け取る
	status = PlayerStatusManager::GetInstance().GetPlayerStatusData().base;

	// ダンジョンによって耐性値減少スピードを変える
	int dungeonID = StageManager::GetInstance().GetCurrentStage()->GetDungeonID();
	resistDownSpeed = dungeonID;

	auto playerModel = LoadManager::GetInstance().LoadResource<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto avoidSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Avoid.mp3");
	auto changeWeaponSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/ChangeWeapon.mp3");
	auto walkSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Work.mp3");
	auto jumpSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Jump.mp3");
	auto bulletHitSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/BulletHit.mp3");
	LoadManager::GetInstance().SetOnComplete([this, playerModel, avoidSE, changeWeaponSE, walkSE, jumpSE, bulletHitSE]() {
		SetModelHandle(playerModel->GetHandle());
		RegisterSEHandle("avoidSE", avoidSE->GetHandle());
		RegisterSEHandle("changeWeaponSE", changeWeaponSE->GetHandle());
		RegisterSEHandle("walkSE", walkSE->GetHandle());
		RegisterSEHandle("jumpSE", jumpSE->GetHandle());
		RegisterSEHandle("bulletHitSE", bulletHitSE->GetHandle());
	});

}

void PlayerComponent::Update(float deltaTime) {
	GameObject* player = GetOwner();
	// プレイヤーの基礎ステータス
	PlayerStatusValue baseStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData().base;
	// プレイヤーの入力情報
	action = GetInputState(GameEnum::ActionMap::PlayerAction);
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
	resistTimePoint -= deltaTime * resistDownSpeed;
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
		auto cameraState = CameraManager::GetInstance().GetCameraState();
		if (cameraState != GameEnum::CameraState::Event && cameraState != GameEnum::CameraState::Invalid) {
			// 視点変更イベント再生
			CameraManager::GetInstance().CameraEventPlay(GameEnum::CameraEvent::Dead);
			// プレイヤーの描画モデル変更
			SetCharacterModel(GetOwner(), playerModelHandle);
			// アニメーションのループをしない
			animator->LoadIndex(false);
			// アニメーション
			animator->Play(static_cast<int>(PlayerAnimNum::Deth), 30);
		}
	}
		
	// 武器変更
	int first = static_cast<int>(GameEnum::PlayerAction::FirstWeapon);
	int second = static_cast<int>(GameEnum::PlayerAction::SecondWeapon);
	if (action.buttonDown[first]) {
		// リボルバーに変更
		WeaponManager::GetInstance().SetCurrentWeapon(GameEnum::Weapon::Revolver);
		// SE再生
		PlaySE("changeWeaponSE");
	}
	if (action.buttonDown[second]) {
		// SMGに変更
		WeaponManager::GetInstance().SetCurrentWeapon(GameEnum::Weapon::SubmachineGun);
		// SE再生
		PlaySE("changeWeaponSE");
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
}

void PlayerComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// プレイヤー以外が発射した弾が当たったらダメージ
	if (auto bullet = other->GetOwner()->GetComponent<BulletComponent>()) {
		if (bullet->GetShotOwner() == GetOwner())
			return;

		status.HP -= bullet->GetHitDamage();
		// SE再生
		PlaySE("bulletHitSE");
	}
}

/*
 *	プレイヤーの移動
 */
void PlayerComponent::PlayerMove(GameObject* player, float deltaTime) {
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();

	// 重力コンポーネントの取得
	auto gravity = player->GetComponent<GravityComponent>();
	// カメラの角度のsin,cos
	const float cameraSin = sinf(camera->rotation.y);
	const float cameraCos = cosf(camera->rotation.y);

	// 移動
	moveVec = V_ZERO;
	float right = action.axis[static_cast<int>(GameEnum::PlayerAction::RightMove)];
	float forward = action.axis[static_cast<int>(GameEnum::PlayerAction::ForwardMove)];
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
	// 2方向に入力されていたら移動量半減
	if (fabs(right) + fabs(forward) == 2) {
		moveVec *= 0.5f;
	}
	player->position.x += moveVec.x;
	player->position.z += moveVec.z;
	// 移動方向を保存
	if (moveVec.x && moveVec.z)
		moveDirectionY = atan2f(moveVec.x, moveVec.z);
	// プレイヤーの向きはカメラに合わせる
	player->rotation.y = camera->rotation.y;
	// 角度を補正
	player->rotation.y += PLAYER_MODEL_ANGLE_CORRECTION * Deg2Rad;

	// ジャンプ
	int jump = static_cast<int>(GameEnum::PlayerAction::Jump);
	if (action.button[jump] && gravity->GetGroundingFrag()) {
		gravity->AddFallSpeed(-JUMP_POWER);
		// SE再生
		PlaySE("jumpSE");
	}

	// アニメーション再生
	if (CameraManager::GetInstance().GetCameraState() == GameEnum::CameraState::TPS) {
		if (moveVec != V_ZERO) {
			if (forward == 1.0f) {
				// 前移動
				animator->Play(static_cast<int>(PlayerAnimNum::Walk), moveSpeed * 0.066f);
			}
			else if (forward == -1.0f) {
				// 後ろ移動
				animator->Play(static_cast<int>(PlayerAnimNum::BackWalk), moveSpeed * 0.066f);
			}
			else if (right == 1.0f) {
				// 左移動
				animator->Play(static_cast<int>(PlayerAnimNum::LeftWalk), moveSpeed * 0.066f);
			}
			else if (right == -1.0f) {
				// 右移動
				animator->Play(static_cast<int>(PlayerAnimNum::RightWalk), moveSpeed * 0.066f);
			}
		}
		else {
			// 待機
			animator->Play(static_cast<int>(PlayerAnimNum::CarryIdle), 1);
		}
	}

	// SE再生
	if (gravity->GetGroundingFrag() && moveVec != V_ZERO) {
		// SE再生クールタイム
		if (walkSECoolTime >= WORK_SE_COOL_TIME_MAX) {
			PlaySE("walkSE");
			walkSECoolTime = 0;
		}
		else {
			walkSECoolTime += deltaTime * moveSpeed * 0.066f;
		}
	}
}

/*
 *	速度調節
 */
void PlayerComponent::SpeedControl(float deltaTime) {
	// ダッシュ
	int run = static_cast<int>(GameEnum::PlayerAction::Run);
	int forwardMove = static_cast<int>(GameEnum::PlayerAction::ForwardMove);
	if (action.button[run] && action.axis[forwardMove] == 1.0f && status.stamina > 0) {
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
	if (action.axis[forwardMove] == -1.0f) {
		acceleration = BACK_ACCELERATION;
	}
	moveSpeed = DEFAULT_MOVE_SPEED * acceleration;
}

/*
 *	回避
 */
void PlayerComponent::PlayerAvoid(GameObject* player, float deltaTime) {
	// 回避開始
	int avoid = static_cast<int>(GameEnum::PlayerAction::Avoid);
	if (action.button[avoid] && canAvoid && status.stamina > STAMINA_AVOID_COST) {
		canAvoid = false;
		isAvoid = true;
		moveSpeed = DEFAULT_MOVE_SPEED * AVOID_ACCELERATION_MAX;
		// スタミナ消費
		status.stamina -= STAMINA_AVOID_COST;
		staminaHealCoolTime = STAMINA_HEAL_COOL_TIME_MAX;
		// SE再生
		PlaySE("avoidSE");
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
 *	モデルセット
 */
void PlayerComponent::SetModelHandle(int setModelHandle) {
	playerModelHandle = setModelHandle;
}