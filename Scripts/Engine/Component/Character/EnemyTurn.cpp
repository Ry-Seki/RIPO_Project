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
	character = 
	enemy = CharacterManager::GetInstance().GetCharacterOwner(character)
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(GameObjectPtr& enemy, float deltaTime)
{
	enemy = 
	auto e = GetOwnerObject(enemy);
	if (enemy->GetChaseTargetChangeFrag()) {
		enemy.SetEnemyRotation(enemy.GetEnemyRotation().y -= ROTATE_SPEED * deltaTime);
	}
	else {
		e->rotation.y += ROTATE_SPEED * deltaTime;
	}
}