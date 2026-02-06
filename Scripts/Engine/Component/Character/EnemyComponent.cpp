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
#include "EnemyDeath.h"
#include "BulletComponent.h"
#include "HPBarComponent.h"
#include "../CameraComponent.h"
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
	, animator(nullptr)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, chaseTargetChangeFrag(false)
	, closePlayer(false)
	, attackIsTriger(false)
	, damageIsTriger(false)
	, turnDelay(0.0f)
	, modelHandle(0)
	, coolTime(2.0f)
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
	state->Start(enemy);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// 仮モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);


	animator->LoadIndex(true);

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
	if (!attackIsTriger && other->GetOwner()->name == "Player") {
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		// 今はとりあえず適当なダメージ
		playerStatus.HP = playerStatus.HP - 10;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		attackIsTriger = true;
	}
	else if (coolTime <= 0) {
		attackIsTriger = false;
		coolTime = 2;
	}

	// 死亡判定
	if (!damageIsTriger && other->GetOwner()->name == "bullet") {
		damageIsTriger = true;
		if (state != nullptr)
			state = new EnemyDeath();
		state->Start(enemy);
	}
}

Vector3 EnemyComponent::DxForwardDir(const Vector3& rotation)
{
	float pitch = rotation.x; // rad
	float yaw = rotation.y; // rad

	Vector3 dir;
	dir.x = -sinf(yaw) * cosf(pitch);
	dir.y = sinf(pitch);
	dir.z = -cosf(yaw) * cosf(pitch);
	return dir.Normalized();
}
