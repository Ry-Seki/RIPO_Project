/*
 *	@file	BossShootingAttack.h
 *	@author kuu
 */
#ifndef _BOSSSHOOTINGATTACK_H_
#define _BOSSSHOOTINGATTACK_H_

#include "BossState.h"
#include "../AnimatorComponent.h"

/*
 *	ボスの攻撃処理クラス
 */
	class BossShootingAttack : public BossState {
	private:
		std::shared_ptr<AnimatorComponent> animator;
		GameObjectPtr player;
		float coolTime;
		// 弾撃ちフラグ
		bool shootFlag;

		const float MAX_COOL_TIME;

	public:
		/*
		 *	コンストラクタ
		 */
		BossShootingAttack();
		~BossShootingAttack() = default;

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

#endif // !_BOSSSHOOTINGATTACK_H_
