/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"

/*
 *	コンストラクタ
 */
EnemyComponent::EnemyComponent()
	: moveSpeed(200.0f)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(200.0f)
	, enemy(nullptr)
	, chaseTargetChangeFrag(false)
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

	/*if (enemy->position.z <= wayPoint.z) {
		wayPoint = enemy->position - nextWayPoint;
		enemy->rotation.y += 180 * Deg2Rad;
	}*/

		EnemyMove(enemy, deltaTime);
}

/*
 *	移動処理
 */
void EnemyComponent::EnemyMove(GameObject* enemy, float deltaTime) {
	const float enemyCos = cos(enemy->rotation.y);
	const float enemySin = sin(enemy->rotation.y);

	if (!chaseTargetChangeFrag) {
		Vector3 direction = wayPoint - enemy->position;
		Vector3 normDirection = Normalized(direction);
		enemy->position += normDirection * moveSpeed * deltaTime;
		if (Magnitude(direction) < 0.1f) {
			chaseTargetChangeFrag = true;
		}
	}
	else if (chaseTargetChangeFrag) {
		Vector3 direction = nextWayPoint - enemy->position;
		Vector3 normDirection = Normalized(direction);
		enemy->position += normDirection * moveSpeed * deltaTime;
		if (Magnitude(direction) < 0.01f) {
			chaseTargetChangeFrag = false;
		}
	}

	//enemy->position.x -= moveSpeed * enemySin;
	//enemy->position.z -= moveSpeed * enemyCos;

}
