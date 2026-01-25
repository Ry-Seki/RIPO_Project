#include "EnemyDeath.h"

/*
 *　コンストラクタ
 */
EnemyDeath::EnemyDeath()
	: animator(nullptr)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyDeath::Start(GameObject* enemy)
{
	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyDeath::Update(GameObject* enemy, float deltaTime)
{
}


