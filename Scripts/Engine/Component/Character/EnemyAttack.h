/*
 *	@file	EnemyAttack.h
 *  @author kuu
 */
#ifndef _ENEMYATTACK_H_
#define _ENEMYATTACK_H_

#include "EnemyComponent.h"
#include "EnemyState.h"

/*
 *	敵の攻撃処理クラス
 */
class EnemyAttack : public EnemyState {
private:
	GameObjectPtr player;
	// クールタイム
	float coolTime;

	// クールタイム最大値
	const float MAX_COOL_TIME;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyAttack();
	~EnemyAttack() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 *  param[in]	EnemyComponent&	enemy
	 */
	virtual void Start(EnemyComponent& enemy) override;

	/*
	 *	更新処理
	 *  param[in]	GameObject*	enemy
	 *  param[in]	float		deltaTime
	 */
	virtual void Update(GameObject* enemy, float deltaTime) override;

public:
	/*
	 *	クールタイムの取得
	 */
	inline float GetCoolTime() const { return coolTime; }
};


#endif // !_ENEMYATTACK_H_
