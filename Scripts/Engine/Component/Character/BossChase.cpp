/*
 *	@file	BossChase.cpp
 *  @author	kuu
 */
#include "BossChase.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "BossStandby.h"
#include "BossAttack.h"

/*
 *	コンストラクタ
 */
BossChase::BossChase()
	: player(nullptr)
	, animator(nullptr)
	, modelHandle(-1)
	, PLAYER_DISTANCE(1000.0)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossChase::Start(GameObject* boss)
{
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossChase::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Update(deltaTime);
	animator->Play(11, 10);

	auto bossComponent = boss->GetComponent<BossComponent>();
	// 状態遷移
	if (!Vision(bossComponent->GetBossPosition(), bossComponent->GetBossRotation(), player->position, 30, 4000)) {
		// 一旦止まってもらう
		boss->GetComponent<BossComponent>()->SetState(new BossStandby());
	}
	// 射程距離判定
	if (PLAYER_DISTANCE > Distance(player->position, boss->position)) {
		boss->GetComponent<BossComponent>()->SetState(new BossAttack());
	}
}