/*
 *	@file	BossStandby.h
 *	@author	kuu
 */
#include "BossStandby.h"
#include "../ModelRenderer.h"
#include "../../Vision.h"
#include "BossComponent.h"
#include "../../Manager/CameraManager.h"
#include "BossChase.h"
#include "BossShootingAttack.h"

/*
 *	コンストラクタ
 */
BossStandby::BossStandby()
	: player(nullptr)
	, animator(nullptr)
	, modelHandle(-1)
	, viewAngle(4000)
	, elapsedTime(0)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossStandby::Start(GameObject* boss)
{
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;

	if (boss->GetComponent<BossComponent>()->GetMoveFrag()) {
		viewAngle = 10000;
	}

	elapsedTime = 0;
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossStandby::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelHandle = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelHandle == -1) return;
	animator->SetModelHandle(modelHandle);

	auto bossComponent = boss->GetComponent<BossComponent>();

	elapsedTime += deltaTime;

	switch (bossComponent->GetBossID())
	{
	case 102:

		// 見失ったら初期位置へ
		if (elapsedTime > 10) {
			boss->position = bossComponent->GetHomePosition();
		}

		// 状態遷移
		if (Vision(boss, -ForwardDir(boss->rotation), player->position, 180, viewAngle)) {
			boss->GetComponent<BossComponent>()->SetState(new BossChase());
		}

		break;
	case 103:
		animator->Play(3, 1 * deltaTime);

		bossComponent->SetOutVisionTime(bossComponent->GetOutVisionTime() + deltaTime);

		// 状態遷移
		if (Vision(boss, -ForwardDir(boss->rotation), player->position, 180, viewAngle)) {
			boss->GetComponent<BossComponent>()->SetState(new BossShootingAttack());
		}

		break;
	default:

		// 状態遷移
		if (Vision(boss, -ForwardDir(boss->rotation), player->position, 180, viewAngle)) {
			boss->GetComponent<BossComponent>()->SetState(new BossChase());
		}

		break;
	}
}