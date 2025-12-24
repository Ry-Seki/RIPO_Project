/*
 *	@file	EnemyTurn.cpp
 *	@author kuu
 */
#include "EnemyTurn.h"
#include "EnemyChase.h"
#include "../../Manager/CharacterManager.h"
#include "CharacterUtility.h"
#include "EnemyAttack.h"

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
	printfDx("EnemyTurn");
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(GameObject* enemy, float deltaTime)
{
	// デバッグ用
	if (CheckHitKey(KEY_INPUT_E)) {
		//enemy->GetComponent<EnemyComponent>()->SetState(new EnemyAttackBase());
	}
	//auto e = GetOwnerObject(enemy);
	if (enemy->GetComponent<EnemyComponent>()->GetChaseTargetChangeFrag()) {
		enemy->rotation.y -= ROTATE_SPEED * deltaTime;
	}
	else {
		enemy->rotation.y += ROTATE_SPEED * deltaTime;
	}
}