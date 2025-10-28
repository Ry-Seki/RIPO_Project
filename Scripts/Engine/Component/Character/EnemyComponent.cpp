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
	// 敵の方向に併せて値を変えられるようにしたいが今は仮
	, nextWayPoint(0.0f, 0.0f, 20.0f)
{
}

void EnemyComponent::Start() {
	wayPoint = nextWayPoint;
}

/*
 *	更新処理
 */
void EnemyComponent::Update(float deltaTime) {
	GameObject* enemy = GetOwner();
	if (enemy->position == wayPoint) {
		wayPoint = enemy->position + nextWayPoint;
	}
	else {
		EnemyMove(enemy, deltaTime);
	}
}

/*
 *	移動処理
 */
void EnemyComponent::EnemyMove(GameObject* enemy, float deltaTime) {
	enemy->position.z  -= moveSpeed;

}
