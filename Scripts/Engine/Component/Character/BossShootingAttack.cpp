/*
 *	@file	BossShootingAttack.h
 *	@author	kuu
 */
#include "BossShootingAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/CameraManager.h"

BossShootingAttack::BossShootingAttack()
	: animator(nullptr)
	, player(nullptr)
	, coolTime(0)
	, MAX_COOL_TIME(2.5f)
{
}

void BossShootingAttack::Start(GameObject* boss)
{
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
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
		Vector3 direction = Direction(boss->position, player->position);
		// 弾発射
		BulletManager::GetInstance().BulletShot(
			boss->position,
			boss->rotation,
			{ 1.0f, 1.0f, 1.0f },
			direction,
			boss,
			1000,
			boss->GetComponent<BossComponent>()->GetBossAttack()
		);
	}
	if (coolTime <= 0) {
		// 状態遷移
		boss->GetComponent<BossComponent>()->SetState(new BossStandby());
	}
}
