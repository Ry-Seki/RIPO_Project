/*
 *	@file	BossStandby.h
 *	@author	kuu
 */
#include "BossStandby.h"
#include "../ModelRenderer.h"

/*
 *	コンストラクタ
 */
BossStandby::BossStandby()
	: animator(nullptr)
	, modelHandle(-1)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossStandby::Start(GameObject* boss)
{
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
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Update(deltaTime);
	animator->Play(7, 10);
}