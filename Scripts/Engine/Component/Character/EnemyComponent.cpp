/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/StageManager.h"
#include "../../Manager/EffectManager.h"
#include "EnemyAttack.h"
#include "EnemyChase.h"
#include "EnemyTurn.h"
#include "EnemyDeath.h"
#include "EnemyStandby.h"
#include "BulletComponent.h"
#include "HPBarComponent.h"
#include "../CameraComponent.h"
 /*
  *	コンストラクタ
  */
EnemyComponent::EnemyComponent()
	: enemy(nullptr)
	, state(nullptr)
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
	, HP(0)
	, coolTime(1.0f)
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

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// 仮モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);

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

	VECTOR position = ToVECTOR(enemy->position);

}

/*
 *	衝突が起きたときに呼び出される処理
 */
void EnemyComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)
{
	if (coolTime <= 0) {
		attackIsTriger = false;
	}
	if (!attackIsTriger && other->GetOwner()->name == "Player") {
		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("EnemyHitEffect", self->GetOwner()->position);
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		playerStatus.HP = playerStatus.HP - status.attack;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		attackIsTriger = true;
	}

	// ダメージ判定
	if (!damageIsTriger && other->GetOwner()->name == "bullet") {
		// ダメージを受ける
		HP -= other->GetOwner()->GetComponent<BulletComponent>()->GetHitDamage();;
		if (HP <= 0) {
			HP = 0;
		}
		// 死亡判定
		if (HP <= 0 && state != nullptr) {
			damageIsTriger = true;
			state = new EnemyDeath();
			state->Start(enemy);
		}
		// 死ななかった場合
		else {
			firstAttackFlag = true;
			hitFlag = true;
		}
	}
	coolTime = 1;
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

void EnemyComponent::SetEnemyStart(int ID)
{
	// 敵のデータ取得
 	switch (ID)
	{
	case 0:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::TutorialEnemy);
		break;
	case 1:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage1Enemy);
		break;
	case 2:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage2Enemy);
		break;
	case 3:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage3Enemy);
		break;
	case 4:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage4Enemy);
		break;
	}
	HP = status.HP;

	if (state == nullptr) {
		// IDに応じて処理を変える
		if (status.ID == 0) {
			state = new EnemyStandby();
		}
		else {
			state = new EnemyChase();
		}
	}

	state->Start(enemy);

	// HPBarComponent
	enemy->GetComponent<HPBarComponent>()->SetMaxHP(status.HP);
	enemy->GetComponent<HPBarComponent>()->SetDisplayHP();
}
