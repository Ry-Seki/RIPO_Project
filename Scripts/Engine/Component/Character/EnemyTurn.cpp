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
	: enemy(nullptr)
	, ROTATE_SPEED(3.0f)
	, targetAngle(0.0f) {
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyTurn::Start(EnemyComponent& enemy) {

	// 回転の目標位置
	Vector3 targetPos;
	// 移動位置が+の場合
	if (!enemy.GetChaseTargetChangeFrag()) {
		// 通常のWayPointを向く
		targetPos = enemy.GetWayPoint();
	}
	// 移動位置が-の場合
	else {
		// 反対側のWayPointを向く
		targetPos = enemy.GetNextWayPoint();
	}

	// 自身から目標への方向ベクトルを計算
	Vector3 direction = Direction(enemy.GetEnemyPosition(), targetPos);

	// Y軸回転の目標角度をだす
	targetAngle = atan2(direction.x, direction.z);
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(GameObject* enemy, float deltaTime) {
	if (enemy == nullptr)return;

	// 敵のコンポーネントを取得
	auto enemyComponent = enemy->GetComponent<EnemyComponent>();

	// 現在角度との差を求める
	float angleDiff = targetAngle - enemy->rotation.y;


	while (angleDiff > Pi)  angleDiff -= Pi * 2;
	while (angleDiff < -Pi) angleDiff += Pi * 2;

	// 回転が足りなければ
	if (fabs(angleDiff) > Deg2Rad * 1.0f) {
		// 回転方向を決定して徐々に回す
		float rotateDir = (angleDiff > 0) ? 1.0f : -1.0f;
		enemy->rotation.y += rotateDir * ROTATE_SPEED * deltaTime;
	}
	else {
		// 角度を合わせる
		enemy->rotation.y = targetAngle;

		// 移動状態へ遷移
		enemyComponent->SetState(new EnemyChase());
	}
}