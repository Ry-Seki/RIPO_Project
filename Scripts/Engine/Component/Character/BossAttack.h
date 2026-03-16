/*
 *	@file	BossAttack.h
 *	@author kuu
 */
#ifndef _BOSSATTACK_H_
#define _BOSSATTACK_H_

#include "BossState.h"
#include "../AnimatorComponent.h"

/*
 *	ボスの攻撃処理クラス
 */
class BossAttack : public BossState {
private:
	std::shared_ptr<AnimatorComponent> animator;
	std::shared_ptr<BossComponent> bossComponent;
	float coolTime;
	float elapsedTime;
	// エフェクトを一回しか出さないためのフラグ
	bool FirstEffectFlag;
	// 音を一回しか鳴らさないためのフラグ
	bool FirstSEFlag;
	// 突進アニメーションの番号
	int headlongAnimation;
	// プレイヤーの方向保存用
	Vector3 playerDirection;

	const float ANIMATION_SPEED;
	// 移動スピード
	const float MOVE_SPEED;

public:
	/*
	 *	コンストラクタ
	 */
	BossAttack();
	~BossAttack() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 *  param[in]	BossComponent&	boss
	 */
	virtual void Start(GameObject* boss) override;

	/*
	 *	更新処理
	 *  param[in]	GameObject*	boss
	 *  param[in]	float		deltaTime
	 */
	virtual void Update(GameObject* boss, float deltaTime) override;

private:
	/*
	 *	範囲攻撃
	 *	param[in]	GameObject* boss
	 *	param[in]	float		deltaTime
	 */
	void RangeAttack(GameObject* boss, float deltaTime);

	/*
	 *	前方攻撃
	 *	param[in]	GameObject* boss
	 *	param[in]	float		deltaTime
	 */
	void ForwardAttack(GameObject* boss, float deltaTime);

	/*
	 *	突進攻撃
	 *	param[in]	GameObject* boss
	 *	param[in]	float		deltaTime
	 *	param[in]	float		attackStateTime
	 */
	void HeadlongAttack(GameObject* boss, float deltaTime, float attackStateTime);
};

#endif // !_BOSSATTACK_H_