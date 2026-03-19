/*
 *	@file	PlayerComponent.h
 *  @author	Riku
 */
#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "../Component.h"
#include "../AnimatorComponent.h"
#include "../MoveComponent.h"
#include "../HPComponent.h"
#include "../StaminaComponent.h"
#include "../StrengthComponent.h"
#include "../ResistTimeComponent.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../Engine.h"
#include "../../GameEnum.h"
#include "../../Input/ActionMapBase.h"

class PlayerComponent : public Component {
private:
	float moveSpeed;								// 移動速度
	float acceleration;								// 加速度
	float avoidMoveValue;							// 回避の移動量
	float avoidCoolTime;							// 回避のクールタイム
	float walkSECoolTime;							// 歩きSEのクールタイム
	Vector3 lastMoveDirection;						// 最後に動いた方向
	Vector3 moveVec;								// プレイヤーの移動量
	bool canAvoid;									// 回避可能か否か
	bool isAvoid;									// 回避中か否か
	bool hasResolvedInitialGrounding;				// 初期接地処理を行ったかどうか
	bool isDead;									// 死亡しているか否か
	int playerModelHandle;							// プレイヤーのモデルハンドル
	AnimatorComponentPtr animator;					// アニメーターコンポーネント
	MoveComponentPtr move;							// 移動コンポーネント
	HPComponentPtr HP;								// HPコンポーネント
	StaminaComponentPtr stamina;					// スタミナコンポーネント
	StrengthComponentPtr strength;					// ストレングスコンポーネント
	ResistTimeComponentPtr resist;					// レジストタイムコンポーネント
	ActionMapBase::ActionState action;				// プレイヤーのアクション状態

	const float PLAYER_MODEL_ANGLE_CORRECTION;	// プレイヤーのモデル角度を補正する値
	const float DEFAULT_MOVE_SPEED;				// デフォルトの移動速度
	const float ACCELERATION_RATE;				// 加速度倍率
	const float RUN_ACCELERATION_MAX;			// ダッシュの最大加速度
	const float AVOID_ACCELERATION_MAX;			// 回避の最大加速度
	const float AVOID_MOVE_VALUE_MAX;			// 回避の最大移動距離
	const float AVOID_COOL_TIME_MAX;			// 回避のクールタイムの最大
	const float STAMINA_RUN_COST;				// ダッシュによるスタミナ消費量
	const float STAMINA_AVOID_COST;				// 回避によるスタミナ消費量
	const float JUMP_POWER;						// ジャンプ力
	const float BACK_ACCELERATION;				// 後ろ歩き時の加速度
	const float HP_DECREASE_RATE;				// HPの減少倍率
	const float WALK_SE_COOL_TIME_MAX;			// 歩きSEのクールタイムの最大
	const float WALK_RATE;						// 歩き倍率
	const float RESIST_DOWN_RATE;				// レジスト減少倍率
	const float DEATH_ANIMATION_SPEED;			// 死亡アニメーションのスピード

	// プレイヤーのアニメーション番号
	enum class PlayerAnimNum {
		Invalid = -1,
		Carry,		// 運び開始時アニメーション
		Deth,		// 死亡時アニメーション
		Idle,		// 通常待機アニメーション
		CarryIdle,	// 運び時待機アニメーション
		Walk,		// 前方歩きアニメーション
		BackWalk,	// 後ろ歩きアニメーション
		LeftWalk,	// 左歩きアニメーション
		RightWalk,	// 右歩きアニメーション
	};

public:
	PlayerComponent();
	~PlayerComponent() = default;

public:
	/*
	 *	最初のUpdateの直前に呼び出される処理
	 */
	void Start() override;
	
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;
	
	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other);

private:
	/*
	 *	プレイヤーの行動
	 */
	void PlayerMove(GameObject* player, float deltaTime);
	
	/*
	 *	速度調節
	 */
	void SpeedControl(GameObject* player, float deltaTime);
	
	/*
	 *	回避
	 */
	void PlayerAvoid(GameObject* player, float deltaTime);
	
	/*
	 *	死亡した時に呼ばれる処理
	 */
	void OnDead();

	/*
	 *	武器変更
	 */
	void ChangeWeapon();

	/*
	 *	モデルセット
	 */
	void SetModelHandle(int setModelHandle);

public:
	/*
	 *	移動量の取得
	 */
	inline Vector3 GetMoveVec()const { return moveVec; }
	/*
	 *	死亡フラグの取得
	 */
	inline bool GetIsDead()const { return isDead; }
	/*
	 *	死亡フラグセット
	 */
	inline void SetIsDead(bool setIsDead) { isDead = setIsDead; }
	
};

#endif // !_PLAYERCOMPONENT_H_
