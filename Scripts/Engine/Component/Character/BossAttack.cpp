/*
 *	@file	BossAttack.cpp
 *	@author	kuu
 */
#include "BossAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"
#include "../../Manager/EffectManager.h"
#include "../../Manager/CameraManager.h"


 /*
  *	コンストラクタ
  */
BossAttack::BossAttack()
	: animator(nullptr)
	, bossComponent(nullptr)
	, player(nullptr)
	, coolTime(0)
	, elapsedTime(0)
	, SEVolume(0)
	, baseSEVolume(0)
	, coolTimeSE(0)
	, FirstEffectFlag(false)
	, FirstSEFlag(false)
	, standbyAnimation(0)
	, headlongAnimation(0)
	, forwardAttackAnimation(0)
	, playerDirection(Vector3::zero)
	, firstPlayerDirection(Vector3::zero)
	, ANIMATION_SPEED(1250)
	, MOVE_SPEED(6000.0f)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossAttack::Start(GameObject* boss)
{
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	bossComponent = boss->GetComponent<BossComponent>();
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	switch (bossComponent->GetBossID())
	{
	case 101:

		coolTime = 3;
		headlongAnimation = 11;

		break;

	case 102:

		coolTime = 2;
		elapsedTime = 0;
		standbyAnimation = 0;
		headlongAnimation = 4;
		forwardAttackAnimation = 1;
		coolTimeSE = 0.1f;

		break;

	case 104:

		coolTime = 2;
		elapsedTime = 0;
		standbyAnimation = 4;
		forwardAttackAnimation = 0;
		headlongAnimation = 7;

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
void BossAttack::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	// プレイヤーとの距離
	auto playerDistance = Distance(player->position, boss->position);
	baseSEVolume = AudioUtility::GetSEVolume();
	// 1～0に変換する
	SEVolume = 1.0f - (playerDistance / 10000);
	SEVolume = SEVolume * baseSEVolume;
	if (SEVolume < 0) {
		SEVolume = 0;
	}

	coolTime -= deltaTime;
	coolTimeSE -= deltaTime;

	// 攻撃中は被ダメ判定は取らない
	bossComponent->SetHitFlag(true);

	switch (bossComponent->GetBossID())
	{
	case 101:

		animator->Play(3, ANIMATION_SPEED * deltaTime);
		RangeAttack(boss, deltaTime);

		break;

	case 102:

		if (bossComponent->GetCloseRangeAttackDistanceFlag()) {
			ForwardAttack(boss, deltaTime);
		}
		else if (bossComponent->GetLongRangeAttackDistanceFlag()) {
			HeadlongAttack(boss, deltaTime, 1.5f);
		}

		break;
	case 104:

		if (bossComponent->GetCloseRangeAttackDistanceFlag()) {
			ForwardAttack(boss, deltaTime);
		}
		else if (bossComponent->GetLongRangeAttackDistanceFlag()) {
			HeadlongAttack(boss, deltaTime, 0.5f);
		}
		else if (bossComponent->GetRangeAttackFlag()) {
			// プレイヤーの場所まで移動する
			auto posX = bossComponent->GetBossToPlayerDirection().x * MOVE_SPEED * deltaTime;
			auto posZ = bossComponent->GetBossToPlayerDirection().z * MOVE_SPEED * deltaTime;
			boss->position.x += posX;
			boss->position.z += posZ;
			RangeAttack(boss, deltaTime);
		}

		break;
	default:
		break;
	}

}

/*
 *	範囲攻撃
 *	param[in]	GameObject* boss
 *	param[in]	float		deltaTime
 */
void BossAttack::RangeAttack(GameObject* boss, float deltaTime)
{
	// 攻撃の当たり判定
	auto aabbCollider = boss->GetComponent<AABBCollider>();
	Vector3 aabbDirection = { 0, 0, 0 };
	const Vector3 aabbMin = { -500, 0, -500 };
	const Vector3 aabbMax = { 500, 50, 500 };

	// アニメーションが終わるまで待ちたい
	if (coolTime <= 1.85f) {
		if (!FirstEffectFlag) {
			// エフェクトを出す
			EffectManager::GetInstance().Instantiate("BossAttackEffect", boss->position);
			FirstEffectFlag = true;
		}
	}
	if (coolTime <= 1.5f) {
		// 攻撃処理
		aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
		if (!FirstSEFlag) {
			// 効果音を出す
			AudioUtility::PlaySE("bossAttackSE");
			FirstSEFlag = true;
		}
		// 攻撃中判定開始
		bossComponent->SetBossAttackTimeFlag(true);
	}
	if (coolTime <= 1.2f) {
		aabbCollider->aabb = { Vector3::zero, Vector3::zero };
		// 攻撃中判定終了
		bossComponent->SetBossAttackTimeFlag(false);
	}
	if (coolTime <= 0) {
		bossComponent->SetHitFlag(false);
		// 状態遷移
		bossComponent->SetState(new BossStandby());
	}
}

/*
 *	前方攻撃
 *	param[in]	GameObject* boss
 *	param[in]	float		deltaTime
 */
void BossAttack::ForwardAttack(GameObject* boss, float deltaTime)
{
	// AABBコライダーを前方に置く
	auto aabbCollider = boss->GetComponent<AABBCollider>();
	float value = 100;
	Vector3 aabbDirection = { value * bossComponent->GetBossToPlayerDirection().x, 0, value * bossComponent->GetBossToPlayerDirection().z };
	const Vector3 aabbMin = { -70, 0, -70 };
	const Vector3 aabbMax = { 70, 100, 70 };

	if (coolTime <= 1.5f) {
		// 前方に当たり判定を出す
		aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
		if (!FirstSEFlag) {
			// 効果音を出す
			AudioUtility::PlaySE("enemyAttackSE");
			FirstSEFlag = true;
		}
		// 攻撃中判定開始
		bossComponent->SetBossAttackTimeFlag(true);
	}
	if (coolTime <= 1.3f) {
		aabbCollider->aabb = { Vector3::zero, Vector3::zero };
		// 攻撃中判定終了
		bossComponent->SetBossAttackTimeFlag(false);
	}
	if (coolTime <= 0.7f) {
		// 待機アニメーション
		animator->Play(standbyAnimation, 5000 * deltaTime);
	}
	// 待機再生中は攻撃アニメーションは再生しない
	else {
		// 攻撃アニメーション
		animator->Play(forwardAttackAnimation, 5000 * deltaTime);
	}
	if (coolTime <= 0) {
		bossComponent->SetCloseRangeAttackDistanceFlag(false);
		// 状態遷移
		bossComponent->SetState(new BossStandby());
	}
}

/*
 *	突進攻撃
 *	param[in]	GameObject* boss
 *	param[in]	float		deltaTime
 */
void BossAttack::HeadlongAttack(GameObject* boss, float deltaTime, float attackStateTime)
{
	// プレイヤーの方向を向く
	if (firstPlayerDirection == Vector3::zero) {
		firstPlayerDirection = bossComponent->GetBossToPlayerDirection();
	}
	boss->rotation.y = atan2(firstPlayerDirection.x, firstPlayerDirection.z) + Pi;
	elapsedTime += deltaTime;

	animator->Play(headlongAnimation, 10 * deltaTime);
	if (elapsedTime >= attackStateTime) {
		if (playerDirection == Vector3::zero) {
			playerDirection = bossComponent->GetBossToPlayerDirection();
		}
		animator->Play(headlongAnimation, 10000 * deltaTime);

		// 攻撃中判定開始
		bossComponent->SetBossAttackTimeFlag(true);
		// プレイヤーがいた方向に突進
		auto posX = playerDirection.x * MOVE_SPEED * deltaTime;
		auto posZ = playerDirection.z * MOVE_SPEED * deltaTime;
		boss->position.x += posX;
		boss->position.z += posZ;

		if (coolTimeSE < 0) {
			// 歩行音を再生
			AudioUtility::SetSEVolume(SEVolume);
			AudioUtility::PlaySE("bossWalkSE");
			AudioUtility::SetSEVolume(baseSEVolume);
			coolTimeSE = 0.1f;
		}

		if (elapsedTime > attackStateTime + 1.5f) {
			// 攻撃中判定終了
			bossComponent->SetBossAttackTimeFlag(false);
			bossComponent->SetLongRangeAttackDistanceFlag(false);
			// 状態遷移
			bossComponent->SetState(new BossStandby());
		}
	}
}


