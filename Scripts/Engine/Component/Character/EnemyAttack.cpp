/*
 *	@file	EnemyAttack.cpp
 *  @author kuu
 */
#include "EnemyAttack.h"
#include "../../Manager/CameraManager.h"
#include "EnemyChase.h"

/*
 *	コンストラクタ
 */
EnemyAttack::EnemyAttack()
	: player(nullptr)
	, coolTime(0)
	, MAX_COOL_TIME(3) {
}

/*
 *	更新処理の前に呼び出す処理
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyAttack::Start(EnemyComponent& enemy)
{
	printfDx("EnemyAttack");
	coolTime = MAX_COOL_TIME;
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	GameObject*	enemy
 *  param[in]	float		deltaTime
 */
void EnemyAttack::Update(GameObject* enemy, float deltaTime)
{
	Vector3 direction = Direction(enemy->position, player->position);
	// AABBコライダーを前方に置く
	auto aabbCollider = enemy->GetComponent<AABBCollider>();
	float value = 100;
	Vector3 aabbDirection = { value * direction.x, 0, value * direction.z };
	const Vector3 aabbMin = { -40, 0, -40 };
	const Vector3 aabbMax = { 40, 100, 40 };
	

	// 前方に当たり判定を出す
	aabbCollider->aabb = { aabbMin + aabbDirection, aabbMax + aabbDirection };

	// 当たったらダメージを与える
	
	coolTime -= deltaTime;
	if (coolTime <= 0) {
		aabbCollider->aabb = { Vector3::zero, Vector3::zero };
		// 状態遷移
		enemy->GetComponent<EnemyComponent>()->SetState(new EnemyChase());
	}
}