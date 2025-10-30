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
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(200.0f)
	, enemy(nullptr)
{
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	wayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z + wayPointDistance);
	nextWayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z - wayPointDistance);
}

/*
 *	更新処理
 */
void EnemyComponent::Update(float deltaTime) {

	if (enemy->position.z <= wayPoint.z) {
		wayPoint = enemy->position - nextWayPoint;
		enemy->rotation.y += 180 * Deg2Rad;
	}

		EnemyMove(enemy, deltaTime);
}

/*
 *	移動処理
 */
void EnemyComponent::EnemyMove(GameObject* enemy, float deltaTime) {
	const float enemyCos = cos(enemy->rotation.y);
	const float enemySin = sin(enemy->rotation.y);

	//Vector3 direction = enemy->position - wayPoint;


	enemy->position.x -= moveSpeed * enemySin;
	enemy->position.z -= moveSpeed * enemyCos;

}
