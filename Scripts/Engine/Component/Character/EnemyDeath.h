/*
 *	@file	EnemyDeath.h
 *	@author	kuu
 */
#ifndef _ENEMYDEATH_H_
#define _ENEMYDEATH_H_

#include "EnemyState.h"
#include "../AnimatorComponent.h"
#include "EnemyComponent.h"

/*
 *	死亡状態処理
 */
class EnemyDeath : public EnemyState {
public:
	EnemyDeath();
	~EnemyDeath() = default;

private:
	std::shared_ptr<AnimatorComponent> animator;
	float animationTime;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 *  param[in]	EnemyComponent&	enemy
	 */
	virtual void Start(GameObject* enemy) override;

	/*
	 *	更新処理
	 *  param[in]	GameObject*	enemy
	 *  param[in]	float		deltaTime
	 */
	virtual void Update(GameObject* enemy, float deltaTime) override;

};

#endif // !_ENEMYDEATH_H_
