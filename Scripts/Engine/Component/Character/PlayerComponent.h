/*
 *	@file	PlayerComponent.h
 *  @author	Riku
 */
#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "CharacterBase.h"
#include "../AnimatorComponent.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../Engine.h"

class PlayerComponent : public CharacterBase {
private:
	float moveSpeed;								// 移動速度
	float acceleration;								// 加速度
	float avoidMoveValue;							// 回避の移動量
	float avoidCoolTime;							// 回避のクールタイム
	float staminaHealCoolTime;						// スタミナが回復し始めるまでの時間
	float staminaChangePoint;						// スタミナの回復＆消費を調節するためのfloat型変数
	float resistTimePoint;							// 耐性値の減少を調節するためのfloat型変数
	float moveDirectionY;							// y軸の移動方向
	Vector3 moveVec;								// プレイヤーの移動量
	bool canAvoid;									// 回避可能か否か
	bool isAvoid;									// 回避中か否か
	bool hasResolvedInitialGrounding;				// 初期接地処理を行ったかどうか
	bool isDead;									// 死亡しているか否か
	std::shared_ptr<AnimatorComponent> animator;	// アニメーターコンポーネント
	PlayerStatusValue status;						// プレイヤーのステータス

	const float PLAYER_MODEL_ANGLE_CORRECTION;	// プレイヤーのモデル角度を補正する値
	const float DEFAULT_MOVE_SPEED;				// デフォルトの移動速度
	const float ACCELERATION_RATE;				// 加速度倍率
	const float RUN_ACCELERATION_MAX;			// ダッシュの最大加速度
	const float AVOID_ACCELERATION_MAX;			// 回避の最大加速度
	const float AVOID_MOVE_VALUE_MAX;			// 回避の最大移動距離
	const float AVOID_COOL_TIME_MAX;			// 回避のクールタイムの最大
	const float STAMINA_HEAL_COOL_TIME_MAX;		// スタミナ回復クールタイムの最大
	const float STAMINA_RUN_COST;				// ダッシュによるスタミナ消費量
	const float STAMINA_AVOID_COST;				// 回避によるスタミナ消費量
	const float STAMINA_HEAL_VALUE;				// スタミナの回復量
	const float JUMP_POWER;						// ジャンプ力
	const float BACK_ACCELERATION;				// 後ろ歩き時の加速度
	const float HP_DECREASE_RATE;				// HPの減少倍率

	// プレイヤーのアニメーション番号
	enum class PlayerAnimNum {
		Invalid = -1,
		Carry = 0,		// 運び開始時アニメーション
		Deth = 1,		// 死亡時アニメーション
		Idle = 2,		// 通常待機アニメーション
		CarryIdle = 3,	// 運び時待機アニメーション
		Walk = 4,		// 前方歩きアニメーション
		BackWalk = 5,	// 後ろ歩きアニメーション
		LeftWalk = 6,	// 左歩きアニメーション
		RightWalk = 7,	// 右歩きアニメーション
	};

public:
	PlayerComponent();
	~PlayerComponent() = default;

public:
	void Start() override;
	void Update(float deltaTime) override;

private:
	/*
	 *	プレイヤーの移動
	 */
	void PlayerMove(GameObject* player, float deltaTime);
	/*
	 *	速度調節
	 */
	void SpeedControl(float deltaTime);
	/*
	 *	回避
	 */
	void PlayerAvoid(GameObject* player, float deltaTime);

	/*
	 *	初期接地チェック
	 *  @param GameObject* player　接地判定をするキャラクター
	 *  @author oorui
	 */
	void IsGrounding(GameObject* player);

public:
	/*
	 *	移動量の取得
	 */
	inline Vector3 GetMoveVec()const { return moveVec; }
	/*
	 *	プレイヤーのステータス取得
	 */
	inline PlayerStatusValue GetPlayerStatus()const { return status; }
	/*
	 *	プレイヤーのステータス更新
	 */
	inline void SetPlayerStatus(PlayerStatusValue setStatus) { status = setStatus; }
	/*
	 *	死亡フラグの取得
	 */
	inline bool GetIsDead()const { return isDead; }
};

#endif // !_PLAYERCOMPONENT_H_
