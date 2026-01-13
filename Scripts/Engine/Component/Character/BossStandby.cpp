/*
 *	@file	BossStandby.h
 *	@author	kuu
 */
#include "BossStandby.h"
#include "../ModelRenderer.h"
#include "../../Vision.h"
#include "BossComponent.h"
#include "../../Manager/CameraManager.h"

/*
 *	コンストラクタ
 */
BossStandby::BossStandby()
	: player(nullptr)
	, animator(nullptr)
	, modelHandle(-1)
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

	animator->Play(7, 10);
	// 視界判定
	if (Vision(bossComponent->GetBossPosition(), bossComponent->GetBossRotation(), player->position, 30, 4000)) {
		//boss->GetComponent<BossComponent>()-> EnemyChase.cppを見よ
	}
}