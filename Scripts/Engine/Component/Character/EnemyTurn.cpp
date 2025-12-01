/*
 *	@file	EnemyTurn.cpp
 *	@author kuu
 */
#include "EnemyTurn.h"
#include "EnemyChase.h"

EnemyTurn::EnemyTurn()
{

}

/*
 *	更新処理の前に呼び出す処理
 */
void EnemyTurn::Start()
{

}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(float deltaTime)
{
	GetChaseTargetChangeFrag()
}
