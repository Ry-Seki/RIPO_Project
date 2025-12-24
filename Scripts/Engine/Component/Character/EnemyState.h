/*
 *	@file	EnemyState.h
 *  @author	kuu
 */
#ifndef _ENEMYSTATE_H_
#define _ENEMYSTATE_H_

#include "../../GameObject.h"

// 前方宣言
class EnemyComponent;

/*
 *	敵行動パターンクラスの基底クラス
 */
class EnemyState {
public:
	EnemyState() = default;
	virtual ~EnemyState() = default;

	virtual void Start(EnemyComponent& enemy) {};
	virtual void Update(GameObject* enemy, float deltaTime) = 0;
	virtual void Execute(EnemyComponent& enemy) {};

};

#endif // !_ENEMYSTATE_H_