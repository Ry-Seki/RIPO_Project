/*
 *	@file	BossDeath.h
 *	@author	kuu
 */
#include "BossDeath.h"
#include "../ModelRenderer.h"
#include "../../System/Money/MoneyManager.h"
#include "CharacterUtility.h"
#include "../../Manager/EffectManager.h"

using namespace CharacterUtility;

/*
 *	コンストラクタ
 */
BossDeath::BossDeath()
	: animator(nullptr)
	, animationTime(0)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossDeath::Start(GameObject* boss)
{
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossDeath::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);
	// アニメーション再生
	animator->Play(6, 30);
	animationTime += deltaTime;
	// アニメーションが終わるまで待つ
	if (animationTime >= 2.4f) {
		// 少量のお金を入手
		MoneyManager::GetInstance().AddMoney(50);
		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("AllEnemyDeathEffect", boss->position);
		// 死亡
		CharacterManager::GetInstance().SetBossDeathFlag(true);
		// 破棄
		RemoveCharacter(boss);
	}
}
