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
	float coolTime;
	// エフェクトを一回しか出さないためのフラグ
	bool FirstEffectFlag;

	const float MAX_COOL_TIME;

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


};

#endif // !_BOSSATTACK_H_