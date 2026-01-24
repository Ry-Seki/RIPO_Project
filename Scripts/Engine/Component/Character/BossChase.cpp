/*
 *	@file	BossChase.cpp
 *  @author	kuu
 */
#include "BossChase.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "BossStandby.h"
#include "BossAttack.h"
#include "../../Stage/StageUtility.h"

using namespace StageUtility;

/*
 *	コンストラクタ
 */
BossChase::BossChase()
	: player(nullptr)
	, animator(nullptr)
	, modelHandle(-1)
	, PLAYER_DISTANCE(1000.0)
	, ROTATE_SPEED(3.0f)
	, MOVE_SPEED(700.0f) {
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossChase::Start(GameObject* boss) {
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;

	boss->GetComponent<BossComponent>()->SetMoveFrag(true);
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossChase::Update(GameObject* boss, float deltaTime) {
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Update(deltaTime);
	animator->Play(11, 10);

	auto bossComponent = boss->GetComponent<BossComponent>();

	ChaseWayPoint(boss, player->position, deltaTime);

	// 状態遷移
	if (!Vision(boss->position, -ForwardDir(boss->rotation), player->position, 30, 4000)) {
		// ボスの生成ID
		//std::vector<int> bossSpawnIDList;
		// ボスの生成位置の取得
		//std::unordered_map<int, Vector3> bossSpawnPos = GetEnemySpwanPos(bossSpawnIDList);

		// 一旦初期位置に戻りきってからstandbyに遷移
		//ChaseWayPoint(boss, bossSpawnPos[0], deltaTime);
		//bossComponent->SetState(new BossStandby());
	}
	// 射程距離判定
	if (PLAYER_DISTANCE > Distance(player->position, boss->position)) {
		bossComponent->SetState(new BossAttack());
	}
}

/*
 *	目標に向かって進む処理
 *  param[in]	GameObject*	boss			ボス
 *  param[in]	Vector3		wayPoint		目標の座標
 *  param[in]	float		deltaTime
 */
void BossChase::ChaseWayPoint(GameObject* boss, Vector3 wayPoint, float deltaTime) {
	// 目標と自身のpositionの差
	const float differenceTarget = 100.0f;
	// 目標の方向
	Vector3 direction = Direction(boss->position, wayPoint);
	float goalAngle = atan2(direction.x, direction.z);
	goalAngle = goalAngle + 180 * Deg2Rad;

	// 正規化した角度の移動量
	float angleDirection = fmod((goalAngle - boss->rotation.y) * Rad2Deg, 360);
	angleDirection = angleDirection * Deg2Rad;

	// 正規化したエネミーの角度
	float bossDirection = fmod(boss->rotation.y * Rad2Deg, 360);
	bossDirection = bossDirection * Deg2Rad;

	Vector3 bossForward = ForwardDir(boss->rotation);

	// -の状態
	if (angleDirection < 0) {
		if (bossDirection > goalAngle) {
			boss->rotation.y -= ROTATE_SPEED * deltaTime;
		}
		else {
			// 目標の方向に補正
			boss->rotation.y = goalAngle;
		}
	}
	// +の状態
	else if (angleDirection > 0) {
		if (bossDirection < goalAngle) {
			boss->rotation.y += ROTATE_SPEED * deltaTime;
		}
		else {
			boss->rotation.y = goalAngle;
		}
	}

	auto distance = Distance(wayPoint, boss->position);
	// プレイヤーの手前で止まる
	if (player && wayPoint == player->position) {
		boss->position.x += direction.x * MOVE_SPEED * deltaTime;
		boss->position.z += direction.z * MOVE_SPEED * deltaTime;
	}
	// 初期位置に戻る時の処理
	else {
		float moveX = 0;
		float moveZ = 0;
		// 目標の方向に進む
		moveX += direction.x * MOVE_SPEED * deltaTime;
		moveZ += direction.z * MOVE_SPEED * deltaTime;

		boss->position.x += moveX;
		boss->position.z += moveZ;
	}
}