/*
 *	@file	EnemyChase.cpp
 *  @author kuu
 */
#include "EnemyChase.h"
#include "EnemyTurn.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "EnemyAttack.h"
#include "../ModelRenderer.h"
#include "EnemyStandby.h"

 /*
  *	コンストラクタ
  */
EnemyChase::EnemyChase()
	: player(nullptr)
	, animator(nullptr)
	, enemyComponent(nullptr)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, moveSpeed(700.0f)
	, closePlayer(false)
	, ROTATE_SPEED(3.0f)
	, DIFFERENCE_PLAYER(700) {
}

void EnemyChase::Start(GameObject* enemy) {
	enemyComponent = enemy->GetComponent<EnemyComponent>();
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	wayPoint = enemyComponent->GetWayPoint();
	nextWayPoint = enemyComponent->GetNextWayPoint();

	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float	deltaTime
 */
void EnemyChase::Update(GameObject* enemy, float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;

	// モデルハンドルのセット
	auto modelRenderer = enemy->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	// 目標判定
	if (player && Vision(enemy->position, -ForwardDir(enemy->rotation), player->position, 30, 2000)) {
		// 足を早くする
		moveSpeed = 1000;
		// アニメーションを再生
		animator->Play(4, 50);
		ChaseWayPoint(enemy, player->position, true, deltaTime);
	}
	else {
		// アニメーションを再生
		animator->Play(7, 20);
		// WayPointを取得
		wayPoint = enemyComponent->GetWayPoint();
		// 反対のWayPointを取得
		nextWayPoint = enemyComponent->GetNextWayPoint();
		// 目標に向かって移動
		if (!enemyComponent->GetChaseTargetChangeFrag()) {
			ChaseWayPoint(enemy, wayPoint, true, deltaTime);
		}
		// 反対の目標に向かって移動
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
	goalAngle = goalAngle + 180 * Deg2Rad;
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

	auto distance = Distance(wayPoint, enemy->position);
	// 標的判定
	if (player && wayPoint == player->position) {
		// 攻撃射程判定
		if (distance > DIFFERENCE_PLAYER) {
			enemy->position.x += direction.x * moveSpeed * deltaTime;
			enemy->position.z += direction.z * moveSpeed * deltaTime;
		}
		else {
			// 攻撃状態遷移
			enemyComponent->SetState(new EnemyAttack());
		}
	}
	else {
		float moveX = 0;
		float moveZ = 0;
		// 目標の方向に進む
		moveX += direction.x * moveSpeed * deltaTime;
		moveZ += direction.z * moveSpeed * deltaTime;

		enemy->position.x += moveX;
		enemy->position.z += moveZ;

		// 移動量を更新
		moveVec = { moveX,0.0f,moveZ };
	}
	// 目標地点についたらターゲットを変える
	if (distance < differenceTarget) {
		enemyComponent->SetChaseTargetChangeFrag(targetChange);
		enemyComponent->SetState(new EnemyStandby());
	}
}
