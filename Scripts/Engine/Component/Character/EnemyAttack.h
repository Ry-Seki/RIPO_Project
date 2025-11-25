/*
 *	@file	EnemyAttack.h
 *  @author kuu
 */
#ifndef _ENEMYATTACK_H_
#define _ENEMYATTACK_H_

/*
 *	敵の攻撃処理基底クラス
 */
class EnemyAttackBase {
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
public:
	void Attack() override {
		// プレイヤーの距離が判定内に入ったら

		// 前方に当たり判定を出す

		// クールタイム

	}
};



#endif // !_ENEMYATTACK_H_
