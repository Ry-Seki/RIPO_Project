/*
 *	@file	EnemyStandby.cpp
 *	@author	kuu
 */
#include "EnemyStandby.h"
#include "../ModelRenderer.h"
#include "EnemyTurn.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "EnemyChase.h"

/*
 *	コンストラクタ
 */
EnemyStandby::EnemyStandby()
	: animator(nullptr)
	, player(nullptr)
	, randStandby(0.0f)
	, elapsedTime(0.0f)
	, randMax(0.0f)
	, RANDOM_MAX(5)
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

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	// ランダム挙動を細かくするための二重乱数
	randMax = GetRand(RANDOM_MAX);
	randStandby = GetRand(randMax * 1000000);
	randStandby = randStandby / 1000000;
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

	// 状況に応じた状態分岐
	if (enemyComponent->GetEnemyDataID() > 0) {
		// ランダム待機
		elapsedTime += deltaTime;
		if (elapsedTime > randStandby) {
			// 移動状態へ遷移
			enemyComponent->SetState(new EnemyTurn());
		}

		// 視界判定
		if (player && Vision(enemy->position, -ForwardDir(enemy->rotation), player->position, 30, 2000)) {
			enemyComponent->SetState(new EnemyChase());
		}
	}
}
