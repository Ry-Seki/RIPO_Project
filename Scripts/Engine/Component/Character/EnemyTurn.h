/*
 *	@file	EnemyTurn.h
 *  @author kuu
 */
#ifndef _ENEMYTURN_H_
#define _ENEMYTURN_H_

#include "EnemyState.h"
#include "../AnimatorComponent.h"

 /*
  *	敵の回転処理クラス
  */
class EnemyTurn : public EnemyState {
private:
	std::shared_ptr<EnemyComponent> enemyComponent;
	std::shared_ptr<AnimatorComponent> animator;
	float targetAngle;		// 回転する際の対象の位置

private:
	// 回転スピード
	const float ROTATE_SPEED;
public:
	EnemyTurn();
	~EnemyTurn() = default;

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

#endif // !_ENEMYTURN_H_
