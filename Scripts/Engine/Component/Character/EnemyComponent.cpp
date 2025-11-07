/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"

/*
 *	コンストラクタ
 */
EnemyComponent::EnemyComponent()
	: moveSpeed(500.0f)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(200.0f)
	, enemy(nullptr)
	, chaseTargetChangeFrag(false)
{
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	if (enemy == nullptr) return;
	wayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z + wayPointDistance);
	nextWayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z - wayPointDistance);
}

/*
 *	更新処理
 */
void EnemyComponent::Update(float deltaTime) {
	// 移動処理
	EnemyMove(enemy, deltaTime);
}

/*
 *	移動処理
 */
void EnemyComponent::EnemyMove(GameObject* enemy, float deltaTime) {
	const float enemyCos = cos(enemy->rotation.y);
	const float enemySin = sin(enemy->rotation.y); 

	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	if (Vision(enemy->position, enemy->rotation, player->position, 60, 2000)) {
		ChaseWayPoint(player->position, true, deltaTime);
	}
	else {
		// 目標に向かって移動
		if (!chaseTargetChangeFrag) {
			ChaseWayPoint(wayPoint, true, deltaTime);
		}
		else if (chaseTargetChangeFrag) {
			ChaseWayPoint(nextWayPoint, false, deltaTime);
		}
	}
	

	//enemy->position.x -= moveSpeed * enemySin;
	//enemy->position.z -= moveSpeed * enemyCos;
}

/*
 *	目標に向かって進む処理
 */
void EnemyComponent::ChaseWayPoint(Vector3 wayPoint, bool targetChange, float deltaTime) {
	// 目標と自身のpositionの差
	const float differenceTarget = 2.0f;
	// 目標の方向
	Vector3 direction = wayPoint - enemy->position;
	// 方向を正規化する
	Vector3 normDirection = Normalized(direction);
	// 目標の方向に進む
	enemy->position += normDirection * moveSpeed * deltaTime;
	// 目標地点についたらターゲットを変える
	if (direction.Magnitude() < differenceTarget) {
		chaseTargetChangeFrag = targetChange;
	}
}
