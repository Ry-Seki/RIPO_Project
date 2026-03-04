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
#include "../../Manager/EffectManager.h"

BossShootingAttack::BossShootingAttack()
	: animator(nullptr)
	, bossComponent(nullptr)
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
	bossComponent = boss->GetComponent<BossComponent>();
	coolTime = MAX_COOL_TIME;
	// エフェクトを出す
	EffectManager::GetInstance().Instantiate("BossShootEffect", boss->position);
	// 射撃待機音を出す
	AudioUtility::PlaySE("bossShootActiveSE");
}

void BossShootingAttack::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	// 攻撃中は被ダメ判定は取らない
	bossComponent->SetHitFlag(true);

	ShootingAttack(boss, deltaTime, 10000);
	
}

void BossShootingAttack::ShootingAttack(GameObject* boss, float deltaTime, float shotSpeed)
{
	animator->Play(0, 2000 * deltaTime);

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
				shotSpeed,
				bossComponent->GetBossAttack()
			);
			// 射撃待機音を出す
			AudioUtility::PlaySE("bossShootAttackSE");
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
