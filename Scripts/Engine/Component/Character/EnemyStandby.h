/*
 *	@file	EnemyStandby.h
 *	@author	kuu
 */
#ifndef _ENEMYSTANDBY_H_
#define _ENEMYSTANDBY_H_

#include "EnemyState.h"
#include "../AnimatorComponent.h"
#include "EnemyComponent.h"

class EnemyStandby : public EnemyState {
private:
	std::shared_ptr<AnimatorComponent> animator;
	// ランダムなスタンバイ時間
	int randStandby;
	// 経過時間
	float elapsedTime;

	// ランダム数の最大値
	const int RANDOM_MAX;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyStandby();
	~EnemyStandby() = default;

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

#endif // !_ENEMYSTANDBY_H_
