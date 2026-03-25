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
	, bossComponent(nullptr)
	, modelHandle(-1)
	, coolTimeSE(0.7f)
	, SEVolume(1.0f)
	, playerDistance(0.0f)
	, moveSpeed(0.0f)
	, animationSpeed(0.0f)
	, closeRangeAttackDistance(0.0f)
	, longRangeAttackDistance(0.0f)
	, headlongCoolTime(0.0f)
	, halfHPFlag(false)
	, ROTATE_SPEED(3.0f)
	, SE_DISTANCE(10000) {
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
	bossComponent = boss->GetComponent<BossComponent>();

	bossComponent->SetMoveFrag(true);

	switch (bossComponent->GetBossID())
	{
	case 101:

		moveSpeed = 700.0f;
		animationSpeed = 1200.0f;
		closeRangeAttackDistance = 1700.0f;
		longRangeAttackDistance = 3000.0f;
		coolTimeSE = 0.7f;

		break;

	case 102:

		moveSpeed = 1000.0f;
		animationSpeed = 1714.29f;
		closeRangeAttackDistance = 700.0f;
		longRangeAttackDistance = 4000.0f;
		coolTimeSE = 0.5f;

		break;

	case 104:

		moveSpeed = 1000.0f;
		animationSpeed = 1650.0f;
		closeRangeAttackDistance = 800.0f;
		longRangeAttackDistance = 4000.0f;
		coolTimeSE = 0.7f;
		headlongCoolTime = 500;

		break;
	default:
		break;
	}
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossChase::Update(GameObject* boss, float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	// プレイヤーとの距離
	playerDistance = Distance(player->position, boss->position);
	auto baseSEVolume = AudioUtility::GetSEVolume();
	// 1～0に変換する
	SEVolume = 1.0f - (playerDistance / SE_DISTANCE);
	SEVolume = SEVolume * baseSEVolume;
	if (SEVolume < 0) {
		SEVolume = 0;
	}

	coolTimeSE -= deltaTime;

	ChaseWayPoint(boss, player->position, deltaTime);
	switch (bossComponent->GetBossID())
	{
	case 101:
		// 歩行アニメーション
		animator->Play(11, animationSpeed * deltaTime);

		// 射程距離判定
		if (closeRangeAttackDistance > Distance(player->position, boss->position)) {
			bossComponent->SetState(new BossAttack());
		}
		else if (longRangeAttackDistance > Distance(player->position, boss->position)) {
			bossComponent->SetState(new BossShootingAttack());
		}

		if (coolTimeSE < 0) {
			// 歩行音を再生
			AudioUtility::SetSEVolume(SEVolume);
			AudioUtility::PlaySE("bossWalkSE");
			AudioUtility::SetSEVolume(baseSEVolume);
			coolTimeSE = 1.5f;
		}

		break;

	case 102:
		// 歩行アニメーション
		animator->Play(3, animationSpeed * deltaTime);

		// 近距離判定内
		if (closeRangeAttackDistance > Distance(player->position, boss->position)) {
			bossComponent->SetCloseRangeAttackDistanceFlag(true);
			bossComponent->SetState(new BossAttack());
		}
		// 遠距離判定外
		if (longRangeAttackDistance < Distance(player->position, boss->position)) {
			bossComponent->SetLongRangeAttackDistanceFlag(true);
			bossComponent->SetState(new BossAttack());
		}

		if (coolTimeSE < 0) {
			// 歩行音を再生
			AudioUtility::SetSEVolume(SEVolume);
			AudioUtility::PlaySE("bossWalkSE");
			AudioUtility::SetSEVolume(baseSEVolume);
			coolTimeSE = 0.85f;
		}
		
		break;

	case 103:

		// ステージ3ボスはchaseしない
		bossComponent->SetState(new BossShootingAttack());

		break;

	case 104:
		// 歩行アニメーション
		animator->Play(7, animationSpeed * deltaTime);

		// HPが半分になった
		if (bossComponent->GetBossHP() <= bossComponent->GetBossMaxHP() / 2) {
			halfHPFlag = true;
			headlongCoolTime = 300;
			moveSpeed = 2000;
		}

		// 範囲攻撃
		/*if (halfHPFlag) {
			if (!bossComponent->GetHPHalfDownFlag()) {
				bossComponent->SetHPHalfDownFlag(true);
				bossComponent->SetRangeAttackFlag(true);
				bossComponent->SetState(new BossAttack());
			}
		}*/

		// ランダムで突進
		if (bossComponent->GetRandomCoolTime() >= headlongCoolTime) {
			bossComponent->SetRandomCoolTime(0);
			bossComponent->SetLongRangeAttackDistanceFlag(true);
			bossComponent->SetState(new BossAttack());
		}
		else {
			// 近距離判定内
			if (closeRangeAttackDistance > Distance(player->position, boss->position)) {
				bossComponent->SetCloseRangeAttackDistanceFlag(true);
				bossComponent->SetState(new BossAttack());
			}
			// 遠距離判定外
			if (longRangeAttackDistance < Distance(player->position, boss->position)) {
				bossComponent->SetState(new BossShootingAttack());
			}
		}

		if (coolTimeSE < 0) {
			// 歩行音を再生
			AudioUtility::SetSEVolume(SEVolume);
			AudioUtility::PlaySE("bossWalkSE");
			AudioUtility::SetSEVolume(baseSEVolume);
			coolTimeSE = 1.0f;
		}

		break;
	default:
		break;
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
	float moveX = 0;
	float moveZ = 0;
	// プレイヤーの手前で止まる
	if (player && wayPoint == player->position) {
		moveX += direction.x * moveSpeed * deltaTime;
		moveZ += direction.z * moveSpeed * deltaTime;
		boss->position.x += moveX;
		boss->position.z += moveZ;
	}
	// 移動量を更新
	moveVec = { moveX,0.0f,moveZ };
}