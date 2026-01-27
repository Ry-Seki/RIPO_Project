/*
 *	@file	EnemyStandby.cpp
 *	@author	kuu
 */
#include "EnemyStandby.h"
#include "../ModelRenderer.h"
#include "EnemyTurn.h"

/*
 *	コンストラクタ
 */
EnemyStandby::EnemyStandby()
	: animator(nullptr)
	, randStandby(0)
	, elapsedTime(0)
	, RANDOM_MAX(3)
{
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyStandby::Start(GameObject* enemy)
{
	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;

	randStandby = GetRand(RANDOM_MAX);
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyStandby::Update(GameObject* enemy, float deltaTime)
{
	// 敵のコンポーネントを取得
	auto enemyComponent = enemy->GetComponent<EnemyComponent>();

	// モデルハンドルのセット
	auto modelRenderer = enemy->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);
	// アニメーションを再生
	animator->Play(3, 30);

	elapsedTime += deltaTime;
	// 状況に応じた状態分岐

	if (elapsedTime > randStandby) {
		// 移動状態へ遷移
		enemyComponent->SetState(new EnemyTurn());
	}
}
