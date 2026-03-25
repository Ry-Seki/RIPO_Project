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
	, lastMoveDirection(V_ZERO)
	, walkSECoolTime(0.0f)
	, canAvoid(true)
	, isAvoid(false)
	, hasResolvedInitialGrounding(false)
	, isDead(false)
	, playerModelHandle(-1)

	, PLAYER_MODEL_ANGLE_CORRECTION(89.98f)
	, DEFAULT_MOVE_SPEED(1500.0f)
	, ACCELERATION_RATE(800.0f)
	, RUN_ACCELERATION_MAX(2.0f)
	, AVOID_ACCELERATION_MAX(6.0f)
	, AVOID_MOVE_VALUE_MAX(1000.0f)
	, AVOID_COOL_TIME_MAX(1.0f)
	, STAMINA_RUN_COST(0.3f)
	, STAMINA_AVOID_COST(10.0f)
	, JUMP_POWER(1400.0f)
	, BACK_ACCELERATION(0.5f)
	, HP_DECREASE_RATE(0.2f)
	, WALK_SE_COOL_TIME_MAX(33.0f)
	, WALK_RATE(0.066f)
	, RESIST_DOWN_RATE(0.7f)
	, DEATH_ANIMATION_SPEED(30.0f)
{}

/*
 *	最初のUpdateの直前に呼び出される処理
 */
void PlayerComponent::Start() {
	// コンポーネント
	auto owner = GetOwner();
	animator = owner->GetComponent<AnimatorComponent>();
	move = owner->GetComponent<MoveComponent>();
	HP = owner->GetComponent<HPComponent>();
	stamina = owner->GetComponent<StaminaComponent>();
	strength = owner->GetComponent<StrengthComponent>();
	resist = owner->GetComponent<ResistTimeComponent>();
	
	// ステータスのセットアップ
	auto status = PlayerStatusManager::GetInstance().GetPlayerStatusData().base;
	GetOwner()->GetComponent<HPComponent>()->Setup(status.HP);
	GetOwner()->GetComponent<StaminaComponent>()->Setup(status.stamina);
	GetOwner()->GetComponent<StrengthComponent>()->Setup(status.strength);
	// ダンジョンによってレジスト値減少スピードを変える
	int dungeonID = StageManager::GetInstance().GetCurrentStage()->GetDungeonID();
	GetOwner()->GetComponent<ResistTimeComponent>()->Setup(status.resistTime, dungeonID * RESIST_DOWN_RATE);
	
	// ロード
	auto playerModel = LoadManager::GetInstance().LoadResource<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto avoidSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Avoid.mp3");
	auto changeWeaponSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/ChangeWeapon.mp3");
	auto walkSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Work.mp3");
	auto jumpSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Jump.mp3");
	auto bulletHitSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/BulletHit.mp3");
	LoadManager::GetInstance().SetOnComplete([this, playerModel, avoidSE, changeWeaponSE, walkSE, jumpSE, bulletHitSE]() {
		SetModelHandle(playerModel->GetHandle());
		RegisterSEHandle(GameConst::_PLAYER_AVOID_SE, avoidSE->GetHandle());
		RegisterSEHandle(GameConst::_CHANGE_WEAPON_SE, changeWeaponSE->GetHandle());
		RegisterSEHandle(GameConst::_PLAYER_WALK_SE, walkSE->GetHandle());
		RegisterSEHandle(GameConst::_PLAYER_JUMP_SE, jumpSE->GetHandle());
		RegisterSEHandle(GameConst::_BULLET_HIT_SE, bulletHitSE->GetHandle());
	});

}

/*
 *	更新処理
 */
void PlayerComponent::Update(float deltaTime) {
	GameObject* player = GetOwner();
	// プレイヤーの入力情報
	action = GetInputState(GameEnum::ActionMap::PlayerAction);
	// 移動量保存
	moveVec = move->GetMoveVec();

	// レジスト値がなくなった場合はHPが割合で削れる
	if (resist->GetResistTime() <= 0) {
		HP->AddDamage(HP->GetMaxHP() * HP_DECREASE_RATE);
	}

	// 死亡処理
	OnDead();
		
	// 武器変更
	ChangeWeapon();

	// 回避
	PlayerAvoid(player, deltaTime);
	// 回避中は処理しない
	if (!isAvoid) {
		// 速度調節
		SpeedControl(player, deltaTime);
		// 移動処理
		PlayerMove(player, deltaTime);
	}
	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(player, moveVec);

	// デバック用カメラ切り替え
#if _DEBUG
	if (action.buttonDown[static_cast<int>(GameEnum::PlayerAction::ChangeDebugCamera)]) {
		// アクションマップの切り替え
		SetActionMapIsActive(GameEnum::ActionMap::DebugCameraAction, true);
		SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, false);
		// カメラステートの切り替え
		CameraManager::GetInstance().SetCameraState(GameEnum::CameraState::Debug);
	}
#endif
}

/*
 *	衝突が起きたときに呼び出される処理
 */
void PlayerComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// プレイヤー以外が発射した弾が当たったらダメージ
	if (auto bullet = other->GetOwner()->GetComponent<BulletComponent>()) {
		if (bullet->GetShotOwner() == GetOwner())
			return;

		HP->AddDamage(bullet->GetHitDamage());
		// SE再生
		PlaySE(GameConst::_BULLET_HIT_SE);
	}
}

/*
 *	プレイヤーの移動
 */
void PlayerComponent::PlayerMove(GameObject* player, float deltaTime) {
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();

	// 移動
	// カメラの角度のsin,cos
	float cameraSin = sinf(camera->rotation.y);
	float cameraCos = cosf(camera->rotation.y);
	// 移動方向ベクトルを出す
	Vector3 moveDir = V_ZERO;
	Vector3 forwardDir = { cameraSin, 0.0f, cameraCos };
	Vector3 rightDir = { cameraCos, 0.0f, -cameraSin };
	float forward = action.axis[static_cast<int>(GameEnum::PlayerAction::ForwardMove)];
	float right = action.axis[static_cast<int>(GameEnum::PlayerAction::RightMove)];
	moveDir += forwardDir * forward;
	moveDir += rightDir * right;
	moveDir = moveDir.Normalized();
	// 移動方向があるなら移動
	if (moveDir != V_ZERO) {
		move->SetVelocity(moveDir, moveSpeed);
		lastMoveDirection = moveDir;
	}

	// プレイヤーの向きはカメラに合わせる
	player->rotation.y = camera->rotation.y;
	// 角度を補正
	player->rotation.y += PLAYER_MODEL_ANGLE_CORRECTION * Deg2Rad;

	// ジャンプ
	auto gravity = player->GetComponent<GravityComponent>();
	int jump = static_cast<int>(GameEnum::PlayerAction::Jump);
	if (action.button[jump] && gravity->GetGroundingFrag()) {
		gravity->AddFallSpeed(-JUMP_POWER);
		// SE再生
		PlaySE(GameConst::_PLAYER_JUMP_SE);
	}

	// アニメーション再生
	if (CameraManager::GetInstance().GetCameraState() == GameEnum::CameraState::TPS) {
		if (moveVec != V_ZERO) {
			if (forward == 1.0f) {
				// 前移動
				animator->Play(static_cast<int>(PlayerAnimNum::Walk), moveSpeed * WALK_RATE);
			}
			else if (forward == -1.0f) {
				// 後ろ移動
				animator->Play(static_cast<int>(PlayerAnimNum::BackWalk), moveSpeed * WALK_RATE);
			}
			else if (right == 1.0f) {
				// 左移動
				animator->Play(static_cast<int>(PlayerAnimNum::LeftWalk), moveSpeed * WALK_RATE);
			}
			else if (right == -1.0f) {
				// 右移動
				animator->Play(static_cast<int>(PlayerAnimNum::RightWalk), moveSpeed * WALK_RATE);
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
		if (walkSECoolTime >= WALK_SE_COOL_TIME_MAX) {
			PlaySE(GameConst::_PLAYER_WALK_SE);
			walkSECoolTime = 0;
		}
		else {
			walkSECoolTime += deltaTime * moveSpeed * WALK_RATE;
		}
	}
}

/*
 *	速度調節
 */
void PlayerComponent::SpeedControl(GameObject* player, float deltaTime) {
	// ダッシュ
	int run = static_cast<int>(GameEnum::PlayerAction::Run);
	int forwardMove = static_cast<int>(GameEnum::PlayerAction::ForwardMove);
	if (action.button[run] && action.axis[forwardMove] == 1.0f && stamina->GetStamina() > 0) {
		// なめらかな加速
		if (acceleration < RUN_ACCELERATION_MAX)
			acceleration += sinf(Deg2Rad * deltaTime * ACCELERATION_RATE);
		else
			acceleration = RUN_ACCELERATION_MAX;

		// スタミナを消費していく
		stamina->UseStamina(STAMINA_RUN_COST);
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
	if (action.button[avoid] && canAvoid && stamina->GetStamina() > STAMINA_AVOID_COST) {
		canAvoid = false;
		isAvoid = true;
		moveSpeed = DEFAULT_MOVE_SPEED * AVOID_ACCELERATION_MAX;
		// スタミナ消費
		stamina->UseStamina(STAMINA_AVOID_COST);
		// SE再生
		PlaySE(GameConst::_PLAYER_AVOID_SE);
	}
	if (isAvoid) {
		// プレイヤーの向いている方向に移動
		move->SetVelocity(lastMoveDirection, moveSpeed);
		// 移動量を加算
		avoidMoveValue += moveSpeed * deltaTime;
		// 特定の距離動いたら回避終了
		if (AVOID_MOVE_VALUE_MAX < avoidMoveValue) {
			// 回避状態のリセット
			ResetAvoid();
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
 *	死亡した時に呼ばれる処理
 */
void PlayerComponent::OnDead() {
	if (HP->IsDead()) {
		auto cameraState = CameraManager::GetInstance().GetCameraState();
		if (cameraState == GameEnum::CameraState::TPS || cameraState == GameEnum::CameraState::FPS) {
			// 視点変更イベント再生
			CameraManager::GetInstance().CameraEventPlay(GameEnum::CameraEvent::Dead);
			// プレイヤーの描画モデル変更
			SetCharacterModel(GetOwner(), playerModelHandle);
			// アニメーションのループをしない
			animator->LoadIndex(false);
			// アニメーション
			animator->Play(static_cast<int>(PlayerAnimNum::Deth), DEATH_ANIMATION_SPEED);
		}
	}
}

/*
 *	武器変更
 */
void PlayerComponent::ChangeWeapon() {
	int first = static_cast<int>(GameEnum::PlayerAction::FirstWeapon);
	int second = static_cast<int>(GameEnum::PlayerAction::SecondWeapon);
	if (action.buttonDown[first]) {
		// リボルバーに変更
		WeaponManager::GetInstance().SetCurrentWeapon(GameEnum::Weapon::Revolver);
		// SE再生
		PlaySE(GameConst::_CHANGE_WEAPON_SE);
	}
	if (action.buttonDown[second]) {
		// SMGに変更
		WeaponManager::GetInstance().SetCurrentWeapon(GameEnum::Weapon::SubmachineGun);
		// SE再生
		PlaySE(GameConst::_CHANGE_WEAPON_SE);
	}
}

/*
 *	モデルセット
 */
void PlayerComponent::SetModelHandle(int setModelHandle) {
	playerModelHandle = setModelHandle;
}

/*
 *	回避状態のリセット
 *  @author oorui
 */
inline void PlayerComponent::ResetAvoid() {
	avoidMoveValue = 0;
	isAvoid = false;
	avoidCoolTime = AVOID_COOL_TIME_MAX;
}

