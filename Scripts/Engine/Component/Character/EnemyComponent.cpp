/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"

/*
 *	コンストラクタ
 */
EnemyComponent::EnemyComponent()
	: moveSpeed(3.0f)
{
}

/*
 *	更新処理
 */
void EnemyComponent::Update(float deltaTime) {
	GameObject* enemy = GetOwner();
	EnemyMove(enemy, deltaTime);
}

/*
 *	移動処理
 */
void EnemyComponent::EnemyMove(GameObject* enemy, float deltaTime) {
	enemy->position.z  -= moveSpeed;

}
