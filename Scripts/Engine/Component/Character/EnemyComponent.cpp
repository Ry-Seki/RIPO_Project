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
	: moveSpeed(700.0f)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(300.0f)
	, enemy(nullptr)
	, chaseTargetChangeFrag(false)
	, turnDelay(0)
	, TOP_VALUE(5000)
	, RANDOM_RANGE(100)
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
 *  param[in]	float	deltaTime
 */
void EnemyComponent::Update(float deltaTime) {
	// モデルが反対なので逆にする
	enemy->rotation.y += 180 * Deg2Rad;
	// 移動処理
	EnemyMove(deltaTime);
	turnDelay += GetRand(RANDOM_RANGE);
}

/*
 *	移動処理
 *  param[in]	float			deltaTime
 */
void EnemyComponent::EnemyMove(float deltaTime) {
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	if (Vision(enemy->position, enemy->rotation, player->position, 30, 2000)) {
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
}

/*
 *	目標に向かって進む処理
 *  param[in]	Vector3		wayPoint		目標の座標
 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
 *  param[in]	float		deltaTime
 */
void EnemyComponent::ChaseWayPoint(Vector3 wayPoint, bool targetChange, float deltaTime) {
	// 目標と自身のpositionの差
	const float differenceTarget = 100.0f;
	// 目標の方向
	Vector3 direction = wayPoint - enemy->position;
	Vector3 normDirection = Normalized(direction);
	// 目標の方向を向く
	enemy->rotation.y = atan2(normDirection.x, normDirection.z);
	/*if (enemy->rotation.y < atan2(normDirection.x, normDirection.z)) {
		enemy->rotation.y += 1 * Deg2Rad;
	}*/
	enemy->rotation.y += 180 * Deg2Rad;
	// 目標地点についたらターゲットを変える
	if (direction.Magnitude() < differenceTarget) {
		// ランダムに待つ
		if (turnDelay > TOP_VALUE) {
			chaseTargetChangeFrag = targetChange;
			turnDelay = 0;
		}
	}
	else {
	// 目標の方向に進む
		enemy->position.x += normDirection.x * moveSpeed * deltaTime;
		enemy->position.z += normDirection.z * moveSpeed * deltaTime;
	}


}