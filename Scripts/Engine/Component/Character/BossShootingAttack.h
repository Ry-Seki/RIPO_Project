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
		std::shared_ptr<BossComponent> bossComponent;
		GameObjectPtr player;
		float coolTime;
		float rapidCoolTime;
		// 弾撃ちフラグ
		bool shootFlag;
		bool secondFlag;
		bool thirdFlag;
		bool slowFlag;
		// プレイヤーの方向
		Vector3 direction;

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

	private:
		/*
		 *	射撃攻撃
		 *	param[in]	GameObject*	boss
		 *  param[in]	float		deltaTime
		 *  param[in]	float		shotSpeed	弾の発射速度
		 */
		void ShootingAttack(GameObject* boss, float deltaTime, float shotSpeed);

		/*
		 *	3点バースト射撃攻撃
		 *	param[in]	GameObject*	boss
		 *  param[in]	float		deltaTime
		 *  param[in]	float		shotSpeed	弾の発射速度
		 */
		void ThreeRoundBurst(GameObject* boss, float deltaTime, float shotSpeed);

		/*
		 *	連射攻撃
		 *	param[in]	GameObject*	boss
		 *  param[in]	float		deltaTime
		 *  param[in]	float		shotSpeed	弾の発射速度
		 */
		void RapidFire(GameObject* boss, float deltaTime, float shotSpeed);

		/*
		 *	サブ弾幕用低速攻撃
		 *	param[in]	GameObject*	boss
		 *  param[in]	float		deltaTime
		 *  param[in]	float		shotSpeed	弾の発射速度
		 *	param[in]	float		coolTime	弾のクールタイム
		 *	param[in]	float		fireTime	coolTime-fireTimeが発射までの時間(coolTimeと同数にすればディレイなし)
		 */
		void SlowBall(GameObject* boss, float deltaTime, float shotSpeed, float coolTime, float fireTime);

};

#endif // !_BOSSSHOOTINGATTACK_H_
