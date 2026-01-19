/*
 *	@file	BossAttack.cpp
 *	@author	kuu
 */
#include "BossAttack.h"
#include "../ModelRenderer.h"
#include "BossComponent.h"
#include "BossStandby.h"

 /*
  *	コンストラクタ
  */
BossAttack::BossAttack()
	: animator(nullptr)
	, modelHandle(-1)
	, coolTime(0)
	, MAX_COOL_TIME(3)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	BossComponent&	boss
 */
void BossAttack::Start(GameObject* boss)
{
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	coolTime = MAX_COOL_TIME;
}

/*
 *	更新処理
 *  param[in]	GameObject*	boss
 *  param[in]	float		deltaTime
 */
void BossAttack::Update(GameObject* boss, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = boss->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	animator->Update(deltaTime);
	animator->Play(3, 10);

	// 攻撃の当たり判定
	auto aabbCollider = boss->GetComponent<AABBCollider>();
	Vector3 aabbDirection = { 0, 0, 0 };
	const Vector3 aabbMin = { -1000, 0, -1000 };
	const Vector3 aabbMax = { 1000, 100, 1000 };
	
	

	// アニメーションが終わるまで待ちたい
	// 仮
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
		// 攻撃処理
		aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
	}
	if (coolTime <= 1.2f) {
		aabbCollider->aabb = { Vector3::zero, Vector3::zero };
	}
	if (coolTime <= 0) {
		// 状態遷移
		boss->GetComponent<BossComponent>()->SetState(new BossStandby());
	}

}
