/*
 *	@file	EnemyDeath.h
 *	@author	kuu
 */
#include "EnemyDeath.h"
#include "../ModelRenderer.h"
#include "../../System/Money/MoneyManager.h"
#include "CharacterUtility.h"
#include "../../Manager/EffectManager.h"

using namespace CharacterUtility;

/*
 *　コンストラクタ
 */
EnemyDeath::EnemyDeath()
	: animator(nullptr)
	, animationTime(0)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyDeath::Start(GameObject* enemy)
{
	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyDeath::Update(GameObject* enemy, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = enemy->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);
	// アニメーションを再生
	animator->Play(2, 30);
	animationTime += deltaTime;
	// アニメーションが終わるまで待つ
	if (animationTime >= 1.0f) {
		// 少量のお金を入手
		MoneyManager::GetInstance().AddMoney(5);
		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("AllEnemyDeathEffect", enemy->position);
		// 破棄
		RemoveCharacter(enemy);
	}
}