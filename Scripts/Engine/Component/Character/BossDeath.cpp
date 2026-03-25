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
	, animationEndTime(0)
	, animationSpeed(0)
	, animationNumber(0)
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

	switch (boss->GetComponent<BossComponent>()->GetBossID())
	{
	case 101:
		animationNumber = 6;
		animationSpeed = 30;
		animationEndTime = 2.4f;
		break;
	case 102:
		animationNumber = 7;
		animationSpeed = 50;
		animationEndTime = 1.07f;
		break;
	case 103:
		animationNumber = 2;
		animationSpeed = 15;
		animationEndTime = 1.3f;
		break;
	case 104:
		animationNumber = 3;
		animationSpeed = 50;
		animationEndTime = 3.0f;
		break;
	default:
		break;
	}
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
	animator->Play(animationNumber, animationSpeed);
	animationTime += deltaTime;
	// アニメーションが終わるまで待つ
	if (animationTime >= animationEndTime) {
		// 少量のお金を入手
		MoneyManager::GetInstance().AddMoney(boss->GetComponent<BossComponent>()->GetBossDataBounty());
		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("AllEnemyDeathEffect", boss->position);
		// 死亡
		CharacterManager::GetInstance().SetBossDeathFlag(true);
		// 破棄
		RemoveCharacter(boss);
	}
}
