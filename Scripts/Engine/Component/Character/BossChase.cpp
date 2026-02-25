/*
 *	@file	BossChase.cpp
 *  @author	kuu
 */
#include "BossChase.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "../../Vision.h"
#include "AllEnemyCommonModule.h"
#include "../../Manager/CameraManager.h"
#include "BossStandby.h"
#include "BossAttack.h"
#include "BossShootingAttack.h"
#include "../../Stage/StageUtility.h"

using namespace StageUtility;

/*
 *	コンストラクタ
 */
BossChase::BossChase()
	: player(nullptr)
	, animator(nullptr)
	, modelHandle(-1)
	, coolTimeSE(0.7f)
	, PLAYER_DISTANCE(1700.0f)
	, SHOOTING_PLAYER_DISTANCE(3000.0f)
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

	coolTimeSE -= deltaTime;
	if (coolTimeSE < 0) {
		// 歩行音を再生
		AudioUtility::PlaySE("bossWalkSE");
		coolTimeSE = 1.5f;
	}

	auto bossComponent = boss->GetComponent<BossComponent>();

	ChaseWayPoint(boss, player->position, deltaTime);

	// 射程距離判定
	if (PLAYER_DISTANCE > Distance(player->position, boss->position)) {
		bossComponent->SetState(new BossAttack());
	}
	else if (SHOOTING_PLAYER_DISTANCE > Distance(player->position, boss->position)) {
		bossComponent->SetState(new BossShootingAttack());
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

	// 反転モデル用
	goalAngle += Pi;
	// 最短の角度
	float angleDiff = EnemyNormalizeAngle(goalAngle - boss->rotation.y);

	float rotateStep = ROTATE_SPEED * deltaTime;

	if (fabs(angleDiff) < rotateStep) {
		boss->rotation.y = goalAngle;
	}
	else {
		boss->rotation.y += (angleDiff > 0 ? rotateStep : -rotateStep);
	}

	auto distance = Distance(wayPoint, boss->position);
	// プレイヤーの手前で止まる
	if (player && wayPoint == player->position) {
		boss->position.x += direction.x * MOVE_SPEED * deltaTime;
		boss->position.z += direction.z * MOVE_SPEED * deltaTime;
	}
}