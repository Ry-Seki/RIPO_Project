/*
 *	@file	BossAttack.cpp
 *	@author	kuu
 */
#include "BossAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"

 /*
  *	コンストラクタ
  */
BossAttack::BossAttack()
	: animator(nullptr)
	, coolTime(0)
	, MAX_COOL_TIME(3)
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

	if (boss->GetComponent<BossComponent>()->GetBossID() == 104) {
		animator->Play(2, 1350 * deltaTime);

		// 攻撃の当たり判定
		auto aabbCollider = boss->GetComponent<AABBCollider>();
		Vector3 aabbDirection = { 0, 0, 0 };
		const Vector3 aabbMin = { -70, 0, -70 };
		const Vector3 aabbMax = { 70, 100, 70 };

		// アニメーションが終わるまで待ちたい
		// 仮
		coolTime -= deltaTime;
		if (coolTime <= 1.5f) {
			// 攻撃処理
			aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
		}
		if (coolTime <= 1.2f) {
			aabbCollider->aabb = { Vector3::zero, Vector3::zero };
		}
		if (coolTime <= 0) {
			// 状態遷移
			boss->GetComponent<BossComponent>()->SetState(new BossStandby());
		}
	}
	else {
		animator->Play(3, 1350 * deltaTime);

		// 攻撃の当たり判定
		auto aabbCollider = boss->GetComponent<AABBCollider>();
		Vector3 aabbDirection = { 0, 0, 0 };
		const Vector3 aabbMin = { -500, 0, -500 };
		const Vector3 aabbMax = { 500, 50, 500 };

		// アニメーションが終わるまで待ちたい
		// 仮
		coolTime -= deltaTime;
		if (coolTime <= 1.5f) {
			// 攻撃処理
			aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
		}
		if (coolTime <= 1.2f) {
			aabbCollider->aabb = { Vector3::zero, Vector3::zero };
		}
		if (coolTime <= 0) {
			// 状態遷移
			boss->GetComponent<BossComponent>()->SetState(new BossStandby());
		}
	}
}