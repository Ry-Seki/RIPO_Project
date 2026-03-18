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
	, rapidCoolTime(0)
	, shootFlag(false)
	, secondFlag(false)
	, thirdFlag(false)
	, slowFlag(false)
	, direction(Vector3::zero)
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

	switch (bossComponent->GetBossID())
	{
	case 101:
		coolTime = MAX_COOL_TIME;

		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("BossShootEffect", boss->position);
		// 射撃待機音を出す
		AudioUtility::PlaySE("bossShootActiveSE");

		break;

	case 103:
		coolTime = MAX_COOL_TIME;

		// HP半分以下で攻撃変化
		if (bossComponent->GetBossHP() <= bossComponent->GetBossMaxHP() / 2) {
			rapidCoolTime = 0.3f;
		}
		else {
			// エフェクトを出す
			EffectManager::GetInstance().Instantiate("BossShootEffect", boss->position);
			// 射撃待機音を出す
			AudioUtility::PlaySE("bossShootActiveSE");
		}

		break;

	case 104:
		coolTime = MAX_COOL_TIME;

		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("BossShootEffect", boss->position);
		// 射撃待機音を出す
		AudioUtility::PlaySE("bossShootActiveSE");

		break;
	default:
		break;
	}
}

void BossShootingAttack::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	// 攻撃中は被ダメ判定は取らない
	bossComponent->SetHitFlag(true);

	direction = bossComponent->GetBossToPlayerDirection();

	switch (bossComponent->GetBossID())
	{
	case 101:

		boss->rotation.y = atan2(direction.x, direction.z) + Pi;
		ShootingAttack(boss, deltaTime, 1000000 * deltaTime);

		break;

	case 103:

		boss->rotation.y = atan2(direction.x, direction.z) + Pi;
		// HP半分以下で攻撃変化
		if (bossComponent->GetBossHP() <= bossComponent->GetBossMaxHP() / 2) {
			SlowBall(boss, deltaTime, 100000 * deltaTime, 0.3f, 0.3f);
			RapidFire(boss, deltaTime, 1000000 * deltaTime);
		}
		else {
			ThreeRoundBurst(boss, deltaTime, 1000000 * deltaTime, 400);
		}

		break;
	case 104:

		boss->rotation.y = atan2(direction.x, direction.z) + Pi;
		SlowBall(boss, deltaTime, 300000 * deltaTime, 0.3f, 0.3f);
		ThreeRoundBurst(boss, deltaTime, 1000000 * deltaTime, 250);

		break;
	default:
		break;
	}
	
}

void BossShootingAttack::ShootingAttack(GameObject* boss, float deltaTime, float shotSpeed)
{
	animator->Play(0, 2000 * deltaTime);

	// アニメーションが終わるまで待ちたい
	// 仮
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
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
			// 射撃音を出す
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

void BossShootingAttack::ThreeRoundBurst(GameObject* boss, float deltaTime, float shotSpeed, float positionY)
{
	animator->Play(0, 2000 * deltaTime);

	// アニメーションが終わるまで待ちたい
	// 仮
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
		if (!shootFlag) {
			// 弾発射
			BulletManager::GetInstance().BulletShot(
				{ boss->position.x, boss->position.y + positionY, boss->position.z },
				boss->rotation,
				{ 1.0f, 1.0f, 1.0f },
				direction,
				boss,
				shotSpeed,
				bossComponent->GetBossAttack()
			);
			// 射撃音を出す
			AudioUtility::PlaySE("bossShootAttackSE");
			shootFlag = true;
		}
	}
	if (coolTime <= 1.2f) {
		if (!secondFlag) {
			// 弾発射
			BulletManager::GetInstance().BulletShot(
				{ boss->position.x, boss->position.y + positionY, boss->position.z },
				boss->rotation,
				{ 1.0f, 1.0f, 1.0f },
				direction,
				boss,
				shotSpeed,
				bossComponent->GetBossAttack()
			);
			// 射撃音を出す
			AudioUtility::PlaySE("bossShootAttackSE");
			secondFlag = true;
		}
	}
	if (coolTime <= 0.9f) {
		if (!thirdFlag) {
			// 弾発射
			BulletManager::GetInstance().BulletShot(
				{ boss->position.x, boss->position.y + positionY, boss->position.z },
				boss->rotation,
				{ 1.0f, 1.0f, 1.0f },
				direction,
				boss,
				shotSpeed,
				bossComponent->GetBossAttack()
			);
			// 射撃音を出す
			AudioUtility::PlaySE("bossShootAttackSE");
			thirdFlag = true;
		}
	}
	if (coolTime <= 0) {
		shootFlag = false;
		bossComponent->SetHitFlag(false);
		// 状態遷移
		bossComponent->SetState(new BossStandby());
	}
}

void BossShootingAttack::RapidFire(GameObject* boss, float deltaTime, float shotSpeed)
{
	animator->Play(0, 20000 * deltaTime);

	// アニメーションが終わるまで待ちたい
	rapidCoolTime -= deltaTime;
	if (rapidCoolTime <= 0.3f) {
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
			// 射撃音を出す
			AudioUtility::PlaySE("bossShootAttackSE");
			shootFlag = true;
		}
	}
	if (rapidCoolTime <= 0) {
		shootFlag = false;
		bossComponent->SetHitFlag(false);
		// 状態遷移
		bossComponent->SetState(new BossStandby());
	}
}

void BossShootingAttack::SlowBall(GameObject* boss, float deltaTime, float shotSpeed, float coolTime, float fireTime)
{
	coolTime -= deltaTime;
	if (coolTime <= fireTime) {
		if (!slowFlag) {
			// 弾発射
			BulletManager::GetInstance().BulletShot(
				{ boss->position.x, boss->position.y + 250, boss->position.z },
				boss->rotation,
				{ 1.0f, 1.0f, 1.0f },
				direction,
				boss,
				shotSpeed,
				bossComponent->GetBossAttack(),
				10
			);
			slowFlag = true;
		}
	}
	if (coolTime <= 0) {
		slowFlag = false;
		bossComponent->SetHitFlag(false);
	}
}
