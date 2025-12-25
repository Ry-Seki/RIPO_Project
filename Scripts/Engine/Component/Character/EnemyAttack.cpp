/*
 *	@file	EnemyAttack.cpp
 *  @author kuu
 */
#include "EnemyAttack.h"

/*
 *	コンストラクタ
 */
EnemyAttack::EnemyAttack()
	: coolTime(3) {
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyAttack::Start(EnemyComponent& enemy)
{
	printfDx("EnemyAttack");
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyAttack::Update(GameObject* enemy, float deltaTime)
{
	coolTime -= deltaTime;
	auto enemyComponent = enemy->GetComponent<EnemyComponent>();
	//enemy = GetOwner();
	if (enemy->GetComponent<EnemyComponent>()->GetClosePlayer()) {
		if (coolTime > 0) return;
		// 前方に当たり判定を出す
		enemy->GetComponent<ColliderBase>()->isCollider = true;
		coolTime = 3;
		// 当たったらダメージを与える
		//AddDamage(3);
	}
	else {
		enemy->GetComponent<ColliderBase>()->isCollider = false;
	}
}