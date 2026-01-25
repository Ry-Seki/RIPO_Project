/*
 *	@file	EnemyDeath.h
 *	@author	kuu
 */
#ifndef _ENEMYDEATH_H_
#define _ENEMYDEATH_H_

#include "EnemyState.h"

/*
 *	死亡状態処理
 */
class EnemyDeath : EnemyState {
public:
	EnemyDeath();
	~EnemyDeath() = default;

private:

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

#endif // !_ENEMYDEATH_H_
