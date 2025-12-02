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
void EnemyTurn::Start()
{
	if (enemy == nullptr) return;
	enemy = GetOwner();
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(float deltaTime)
{
	if (enemy->GetComponent<EnemyComponent>()->GetChaseTargetChangeFrag()) {
		enemy->rotation.y -= ROTATE_SPEED * deltaTime;
	}
	else {
		enemy->rotation.y += ROTATE_SPEED * deltaTime;
	}
}
