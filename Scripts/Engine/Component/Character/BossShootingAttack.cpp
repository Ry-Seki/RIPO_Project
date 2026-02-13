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
	, shootFlag(false)
	, MAX_COOL_TIME(2.3f)
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
	auto bossComponent = boss->GetComponent<BossComponent>();
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Play(0, 2000 * deltaTime);

	bossComponent->SetHitFlag(true);

	// アニメーションが終わるまで待ちたい
	// 仮
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
		// 攻撃処理
		Vector3 direction = Direction(boss->position, player->position);
		if (!shootFlag) {
			// 弾発射
			BulletManager::GetInstance().BulletShot(
				{ boss->position.x, boss->position.y + 250, boss->position.z },
				boss->rotation,
				{ 1.0f, 1.0f, 1.0f },
				direction,
				boss,
				10000,
				bossComponent->GetBossAttack()
			);
			shootFlag = true;
		}
	}
	if (coolTime <= 0) {
		shootFlag = false;
		bossComponent->SetHitFlag(false);
		// 状態遷移
		bossComponent->SetState(new BossStandby());
	}
}
