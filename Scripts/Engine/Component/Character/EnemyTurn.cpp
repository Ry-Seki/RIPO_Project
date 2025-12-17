/*
 *	@file	EnemyTurn.cpp
 *	@author kuu
 */
#include "EnemyTurn.h"
#include "EnemyChase.h"
#include "../../Manager/CharacterManager.h"
#include "CharacterUtility.h"

using namespace CharacterUtility;

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
void EnemyTurn::Update(GameObjectPtr& enemy, float deltaTime)
{
	//auto e = GetOwnerObject(enemy);
	if (enemy->GetComponent<EnemyComponent>()->GetChaseTargetChangeFrag()) {
		enemy->rotation.y -= ROTATE_SPEED * deltaTime;
	}
	else {
		enemy->rotation.y += ROTATE_SPEED * deltaTime;
	}
}