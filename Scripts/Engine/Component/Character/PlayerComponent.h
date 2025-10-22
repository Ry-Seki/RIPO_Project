/*
 *	@file	PlayerComponent.h
 *  @author	Riku
 */
#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "CharacterBase.h"
#include "../../Engine.h"

class PlayerComponent : public CharacterBase {
private:
	float moveSpeed;		// 移動速度
	float acceleration;		// 加速度
	float avoidMoveValue;	// 回避の移動量
	bool canAvoid;			// 回避可能か否か
	bool isAvoid;			// 回避中か否か

	const float DEFAULT_MOVE_SPEED;		// デフォルトの移動速度
	const float ACCELERATION_RATE;		// 加速度倍率
	const float RUN_ACCELERATION_MAX;	// ダッシュの最大加速度
	const float AVOID_ACCELERATION_MAX;	// 回避の最大加速度
	const float AVOID_MOVE_VALUE_MAX;	// 回避の最大移動距離

public:
	PlayerComponent();
	virtual ~PlayerComponent() = default;

public:
	virtual void Update(float deltaTime) override;

private:
	/*
	 *	プレイヤーの移動
	 */
	void PlayerMove(GameObject* player, float deltaTime);
	/*
	 *	ダッシュ
	 */
	void PlayerRun(float deltaTime);
	/*
	 *	回避
	 */
	void PlayerAvoid(GameObject* player, float deltaTime);

};

#endif // !_PLAYERCOMPONENT_H_
