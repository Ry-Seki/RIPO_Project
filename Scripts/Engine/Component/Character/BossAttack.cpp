/*
 *	@file	BossAttack.cpp
 *	@author	kuu
 */
#include "BossAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"
#include "../../Manager/EffectManager.h"


 /*
  *	コンストラクタ
  */
BossAttack::BossAttack()
	: animator(nullptr)
	, bossComponent(nullptr)
	, coolTime(0)
	, FirstEffectFlag(false)
	, FirstSEFlag(false)
	, MAX_COOL_TIME(3)
	, ANIMATION_SPEED(1250)
	, MOVE_SPEED(1500.0f)
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
	coolTime = MAX_COOL_TIME;
	bossComponent = boss->GetComponent<BossComponent>();

	playerDirection = bossComponent->GetBossToPlayerDirection();
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

	coolTime -= deltaTime;

	// 攻撃中は被ダメ判定は取らない
	bossComponent->SetHitFlag(true);

	switch (bossComponent->GetBossID())
	{
	case 101:

		RangeAttack(boss, deltaTime);

		break;

	case 102:

		HeadlongAttack(boss, deltaTime);

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
	animator->Play(3, ANIMATION_SPEED * deltaTime);

	// 攻撃の当たり判定
	auto aabbCollider = boss->GetComponent<AABBCollider>();
	Vector3 aabbDirection = { 0, 0, 0 };
	const Vector3 aabbMin = { -500, 0, -500 };
	const Vector3 aabbMax = { 500, 50, 500 };

	// アニメーションが終わるまで待ちたい
	// 仮
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
 *	範囲攻撃
 *	param[in]	GameObject* boss
 *	param[in]	float		deltaTime
 */
void BossAttack::HeadlongAttack(GameObject* boss, float deltaTime)
{
	animator->Play(3, ANIMATION_SPEED * deltaTime);

	// プレイヤーがいた方向に突進
	auto posX = playerDirection.x * MOVE_SPEED * deltaTime;
	auto posZ = playerDirection.z * MOVE_SPEED * deltaTime;
	boss->position.x += posX;
	boss->position.y += posZ;
}
