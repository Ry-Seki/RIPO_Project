/*
 *	@file	BossShootingAttack.h
 *	@author	kuu
 */
#include "BossShootingAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"
#include "../../Manager/BulletManager.h"

BossShootingAttack::BossShootingAttack()
	: animator(nullptr)
	, coolTime(0)
	, MAX_COOL_TIME(3)
{
}

void BossShootingAttack::Start(GameObject* boss)
{
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	coolTime = MAX_COOL_TIME;
}

void BossShootingAttack::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Play(0, 2000 * deltaTime);

	// アニメーションが終わるまで待ちたい
	// 仮
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
		// 攻撃処理
		BulletManager::GetInstance().GenerateBullet(
			"BossBullet",
			boss->position,
			boss->rotation,
			{ -10, 0, -10 },
			{ 10, 20, 10 }
		);
	}
	if (coolTime <= 0) {
		// 状態遷移
		boss->GetComponent<BossComponent>()->SetState(new BossStandby());
	}
}
