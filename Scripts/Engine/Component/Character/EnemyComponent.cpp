/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/StageManager.h"
 /*
  *	コンストラクタ
  */
EnemyComponent::EnemyComponent()
	: wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, enemy(nullptr)
	, chaseTargetChangeFrag(false)
	, closePlayer(false)
	, turnDelay(0)
	, TOP_VALUE(5000)
	, RANDOM_RANGE(100)
	, ROTATE_SPEED(3.0f)
	, MOVE_SPEED(700.0f)
	, DIFFERENCE_PLAYER(700) {
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	if (enemy == nullptr) return;
	player = CameraManager::GetInstance().GetTarget();
	wayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z + wayPointDistance);
	nextWayPoint = Vector3(enemy->position.x, enemy->position.y, enemy->position.z - wayPointDistance);
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyComponent::Update(float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;

	// モデルが反対なので逆にする
	// 移動処理
	EnemyMove(deltaTime);

	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(enemy, moveVec);
}

/*
 *	移動処理
 *  param[in]	float			deltaTime
 */
void EnemyComponent::EnemyMove(float deltaTime) {
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
	// 目標地点についたらターゲットを変える
	else if (distance < differenceTarget) {
		turnDelay += GetRand(RANDOM_RANGE);
		// ランダムに待つ
		if (turnDelay > TOP_VALUE) {
			chaseTargetChangeFrag = targetChange;
			turnDelay = 0;
		}
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