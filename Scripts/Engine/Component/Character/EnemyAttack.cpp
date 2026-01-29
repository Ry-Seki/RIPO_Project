/*
 *	@file	EnemyAttack.cpp
 *  @author kuu
 */
#include "EnemyAttack.h"
#include "../../Manager/CameraManager.h"
#include "EnemyChase.h"
#include "PlayerComponent.h"
#include "../ModelRenderer.h"

/*
 *	コンストラクタ
 */
EnemyAttack::EnemyAttack()
	: player(nullptr)
	, animator(nullptr)
	, enemyComponent(nullptr)
	, coolTime(0)
	, MAX_COOL_TIME(2) {
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyAttack::Start(GameObject* enemy)
{
	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;

	coolTime = MAX_COOL_TIME;
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	enemy->GetComponent<EnemyComponent>()->SetAttackFlag(true);
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyAttack::Update(GameObject* enemy, float deltaTime)
{
	// モデルハンドルのセット
	auto modelRenderer = enemy->GetComponent<ModelRenderer>()->GetModelHandle();
	if (modelRenderer == -1) return;
	animator->SetModelHandle(modelRenderer);

	Vector3 direction = Direction(enemy->position, player->position);
	// AABBコライダーを前方に置く
	auto aabbCollider = enemy->GetComponent<AABBCollider>();
	float value = 100;
	Vector3 aabbDirection = { value * direction.x, 0, value * direction.z };
	const Vector3 aabbMin = { -70, 0, -70 };
	const Vector3 aabbMax = { 70, 100, 70 };
	
	coolTime -= deltaTime;
	if (coolTime <= 1.5f) {
		// 前方に当たり判定を出す
		aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };
	}
	if (coolTime <= 1.3f) {
		aabbCollider->aabb = { Vector3::zero, Vector3::zero };
	}
	if (coolTime <= 0.7f) {
		// 待機アニメーション
		animator->Play(3, 50);
	}
	// 待機再生中は攻撃アニメーションは再生しない
	else {
		// 攻撃アニメーション
		animator->Play(0, 100);
	}
	if (coolTime <= 0) {
		// 状態遷移
		enemy->GetComponent<EnemyComponent>()->SetState(new EnemyChase());
	}
}