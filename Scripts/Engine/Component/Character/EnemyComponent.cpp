/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/StageManager.h"
#include "EnemyAttack.h"
#include "EnemyChase.h"
#include "EnemyTurn.h"
 /*
  *	コンストラクタ
  */
EnemyComponent::EnemyComponent()
	: EnemyComponent(new EnemyChase())
{
}

EnemyComponent::EnemyComponent(EnemyState* initState)
	: enemy(nullptr)
	, state(initState)
	, player(nullptr)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, chaseTargetChangeFrag(false)
	, closePlayer(false)
	, isTriger(false)
	, turnDelay(0.0f)
	, coolTime(3.0f)
	, TOP_VALUE(5000)
	, RANDOM_RANGE(100)
	, moveVec(Vector3::zero) {
}

EnemyComponent::~EnemyComponent()
{
	delete state;
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	if (enemy == nullptr) return;

	if (state == nullptr)
		state = new EnemyChase();
	state->Start(*this);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyComponent::Update(float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;

	if (state == nullptr || enemy == nullptr) return;
	state->Update(enemy, deltaTime);

	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(enemy, moveVec);

	coolTime -= deltaTime;
}

/*
 *	衝突が起きたときに呼び出される処理
 */
void EnemyComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)
{
	if (!isTriger && other->GetOwner()->name == "Player") {
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		// 今はとりあえず適当なダメージ
		playerStatus.HP = playerStatus.HP - 3;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		isTriger = true;
	}
	else if (coolTime <= 0) {
		isTriger = false;
		coolTime = 3;
	}
}