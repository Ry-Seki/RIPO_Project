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
#include "../../Load/Audio/LoadAudio.h"
#include "../../Audio/AudioUtility.h"

using namespace AudioUtility;

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
	// 効果音の読み込み
	auto bossShootActiveSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossShootActiveSE.mp3");
	auto bossShootAttackSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossShootAttackSE.mp3");
	LoadManager::GetInstance().SetOnComplete([this, bossShootActiveSE, bossShootAttackSE]() {
		AudioUtility::RegisterSEHandle("bossShootActiveSE", bossShootActiveSE->GetHandle());
		AudioUtility::RegisterSEHandle("bossShootAttackSE", bossShootAttackSE->GetHandle());
		});
	// エフェクトを出す
	EffectManager::GetInstance().Instantiate("BossShootEffect", boss->position);
	// 射撃待機音を出す
	PlaySE("bossShootActiveSE");
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
			// 射撃待機音を出す
			PlaySE("bossShootAttackSE");
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
