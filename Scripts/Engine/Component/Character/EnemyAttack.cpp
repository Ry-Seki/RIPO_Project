/*
 *	@file	EnemyAttack.cpp
 *  @author kuu
 */
#include "EnemyAttack.h"

/*
 *	コンストラクタ
 */
EnemyAttack::EnemyAttack()
	: coolTime(0)
	, MAX_COOL_TIME(3) {
	coolTime = MAX_COOL_TIME;
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
	if (enemy->GetComponent<EnemyComponent>()->GetClosePlayer()) {
		if (coolTime > 0) return;
		// 前方に当たり判定を出す
		enemy->GetComponent<ColliderBase>()->isCollider = true;
		coolTime = MAX_COOL_TIME;
		// 当たったらダメージを与える
	}
	else {
		enemy->GetComponent<ColliderBase>()->isCollider = false;
	}
}