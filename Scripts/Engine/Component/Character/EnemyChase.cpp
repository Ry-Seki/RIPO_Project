/*
 *	@file	EnemyChase.cpp
 *  @author kuu
 */
#include "EnemyChase.h"
#include "EnemyTurn.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"

/*
 *	コンストラクタ
 */
EnemyChase::EnemyChase()
	: player(nullptr)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, closePlayer(false)
	, MOVE_SPEED(700.0f)
	, ROTATE_SPEED(3.0f)
	, DIFFERENCE_PLAYER(700) {
}

void EnemyChase::Start(EnemyComponent& enemy)
{
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	wayPoint = Vector3(enemy.GetEnemyPosition().x, enemy.GetEnemyPosition().y, enemy.GetEnemyPosition().z + wayPointDistance);
	nextWayPoint = Vector3(enemy.GetEnemyPosition().x, enemy.GetEnemyPosition().y, enemy.GetEnemyPosition().z - wayPointDistance);
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float	deltaTime
 */
void EnemyChase::Update(GameObject* enemy, float deltaTime)
{
	// 移動量を初期化
	moveVec = Vector3::zero;

	auto enemyComponent = enemy->GetComponent<EnemyComponent>();

	if (player && Vision(enemyComponent->GetEnemyPosition(), enemyComponent->GetEnemyRotation(), player->position, 30, 2000)) {
		ChaseWayPoint(enemy, player->position, true, deltaTime);
	}
	else {
		// 目標に向かって移動
		if (!enemyComponent->GetChaseTargetChangeFrag()) {
			ChaseWayPoint(enemy, wayPoint, true, deltaTime);
		}
		else if (enemyComponent->GetChaseTargetChangeFrag()) {
			ChaseWayPoint(enemy, nextWayPoint, false, deltaTime);
		}
	}
}

/*
 *	目標に向かって進む処理
 *  param[in]	GameObject*	enemy
 *  param[in]	GameObject*	enemy			敵
 *  param[in]	Vector3		wayPoint		目標の座標
 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
 *  param[in]	float		deltaTime
 */
void EnemyChase::ChaseWayPoint(GameObject* enemy, Vector3 wayPoint, bool targetChange, float deltaTime) {
	// デバッグ用
	if (CheckHitKey(KEY_INPUT_Q)) {
		enemy->GetComponent<EnemyComponent>()->SetState(new EnemyTurn());
	}
	// 目標と自身のpositionの差
	const float differenceTarget = 100.0f;
	// 目標の方向
	Vector3 direction = Direction(enemy->position, wayPoint);
	float goalAngle = atan2(direction.x, direction.z);
	//goalAngle += 180 * Deg2Rad;
	// 正規化した角度の移動量
	float angleDirection = fmod((goalAngle - enemy->rotation.y) * Rad2Deg, 360);
	angleDirection = angleDirection * Deg2Rad;

	// 正規化したエネミーの角度
	float enemyDirection = fmod(enemy->rotation.y * Rad2Deg, 360);
	enemyDirection = enemyDirection * Deg2Rad;

	Vector3 enemyForward = ForwardDir(enemy->rotation);

	// -の状態
	if (angleDirection < 0) {
		if (enemyDirection > goalAngle) {
			enemy->rotation.y -= ROTATE_SPEED * deltaTime;
		}
		else {
			// 目標の方向に補正
			enemy->rotation.y = goalAngle;
		}
	}
	// +の状態
	else if (angleDirection > 0) {
		if (enemyDirection < goalAngle) {
			enemy->rotation.y += ROTATE_SPEED * deltaTime;
		}
		else {
			enemy->rotation.y = goalAngle;
		}
	}

	// EnemyAttackに移動するかも(攻撃の直前だけ動かすでよいため)
	// AABBコライダーを前方に置く
	auto aabbCollider = enemy->GetComponent<AABBCollider>();
	float value = 100;
	Vector3 aabbDirection = { value * direction.x, 0, value * direction.z };
	const Vector3 aabbMin = { -40, 0, -40 };
	const Vector3 aabbMax = { 40, 100, 40 };
	aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };

	auto distance = Distance(wayPoint, enemy->position);
	// プレイヤーの手前で止まる
	if (player) {
		if (wayPoint == player->position) {
			if (distance > DIFFERENCE_PLAYER) {
				closePlayer = true;
				enemy->position.x += direction.x * MOVE_SPEED * deltaTime;
				enemy->position.z += direction.z * MOVE_SPEED * deltaTime;
			}
			else {
				closePlayer = false;
			}
		}
	}
	// 目標地点についたらターゲットを変える
	if (distance < differenceTarget) {
		enemy->GetComponent<EnemyComponent>()->SetChaseTargetChangeFrag(targetChange);
		enemy->GetComponent<EnemyComponent>()->SetState(new EnemyTurn());
	}
	else {
		// 目標の方向に進む
		enemy->position.x += direction.x * MOVE_SPEED * deltaTime;
		enemy->position.z += direction.z * MOVE_SPEED * deltaTime;

		float moveX = enemy->position.x;
		float moveY = enemy->position.y;
		// 移動量を更新
		moveVec = { moveX,0.0f,moveY };
	}
}
