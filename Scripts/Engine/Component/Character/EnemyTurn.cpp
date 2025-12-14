/*
 *	@file	EnemyTurn.cpp
 *	@author kuu
 */
#include "EnemyTurn.h"
#include "EnemyChase.h"

EnemyTurn::EnemyTurn()
	:enemy(nullptr)
	, ROTATE_SPEED(3.0f)
{
}

/*
 *	更新処理の前に呼び出す処理
 */
void EnemyTurn::Start(EnemyComponent& enemy)
{
	
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(EnemyComponent& enemy, float deltaTime)
{
	if (enemy.GetChaseTargetChangeFrag()) {
		enemy.SetEnemyRotation(enemy.GetEnemyRotation().y -= ROTATE_SPEED * deltaTime);
	}
	else {
		enemy->rotation.y += ROTATE_SPEED * deltaTime;
	}
}