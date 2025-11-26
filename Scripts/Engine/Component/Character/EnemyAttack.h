/*
 *	@file	EnemyAttack.h
 *  @author kuu
 */
#ifndef _ENEMYATTACK_H_
#define _ENEMYATTACK_H_

#include "EnemyComponent.h"

/*
 *	敵の攻撃処理基底クラス
 */
class EnemyAttackBase : public Component {
public:
	EnemyAttackBase() = default;
	virtual ~EnemyAttackBase() = default;
public:
	virtual void Attack() = 0;
};

/*
 *	通常敵の攻撃処理クラス
 */
class NormalEnemyAttack : public EnemyAttackBase {
	GameObject* enemy;
public:
	void Attack() override {
		enemy = GetOwner();
		if (enemy == nullptr) return;
		if (enemy->GetComponent<EnemyComponent>()->GetClosePlayer()) {
		// 前方に当たり判定を出す

		// クールタイム

		}

	}
};



#endif // !_ENEMYATTACK_H_
