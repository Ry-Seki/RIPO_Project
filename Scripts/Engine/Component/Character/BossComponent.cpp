/*
 *	@file	BossComponent.cpp
 *	@author	kuu
 */
#include "BossComponent.h"
#include "../../Manager/GameObjectManager.h"
#include "BossStandby.h"
#include "PlayerComponent.h"
#include "../../Manager/CameraManager.h"
#include "BossDeath.h"
#include "BossChase.h"
#include "BulletComponent.h"
#include "BossHPBarComponentr.h"
#include "../../Manager/EffectManager.h"

/*
 *	コンストラクタ
 */
BossComponent::BossComponent()
	: BossComponent(new BossStandby())
{
}

BossComponent::BossComponent(BossState* initState)
	: boss(nullptr)
	, player(nullptr)
	, animator(nullptr)
	, state(initState)
	, modelHandle(-1)
	, HP(0)
	, coolTime(3)
	, attackIsTriger(false)
	, damageIsTriger(false)
	, moveFrag(false)
	, hitFlag(false)
	, homePosition(Vector3::zero)
{
}

BossComponent::~BossComponent()
{
	delete state;
}

void BossComponent::Start()
{
	// モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/Boss/Boss.mv1");
	boss = GetOwner();
	if (boss == nullptr) return;
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	homePosition = boss->position;

	if (state == nullptr)
		state = new BossStandby();
	state->Start(boss);

	animator->LoadIndex(true);
}

/*
 *	更新処理
 */
void BossComponent::Update(float deltaTime)
{
	if (state == nullptr || boss == nullptr) return;
	state->Update(boss, deltaTime);

	coolTime -= deltaTime;
}

/*
 *	衝突が起きたときに呼び出される処理
 */
void BossComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)
{
	if (coolTime <= 0) {
		attackIsTriger = false;
	}
	if (!attackIsTriger && other->GetOwner()->name == "Player") {
		// エフェクトを出す
		EffectManager::GetInstance().Instantiate("AllEnemyHitEffect", other->GetOwner()->position);
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		playerStatus.HP = playerStatus.HP - status.attack;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		attackIsTriger = true;
	}
	coolTime = 2;

	// ダメージ判定
	auto otherOwner = other->GetOwner();
	if (!otherOwner) return;

	auto bulletComp = otherOwner->GetComponent<BulletComponent>();
	if (!bulletComp) return;

	if (!damageIsTriger && bulletComp->GetShotOwner() == player.get()) {
		// ダメージを受ける
		HP -= other->GetOwner()->GetComponent<BulletComponent>()->GetHitDamage();;
		if (HP <= 0) {
			HP = 0;
		}
		// 死亡判定
		if (HP <= 0 && state != nullptr) {
			damageIsTriger = true;
			state = new BossDeath();
			state->Start(boss);
		}
		// 死ななかった場合
		else {
			if (!hitFlag) {
				state = new BossChase();
				state->Start(boss);
				hitFlag = true;
			}
		}
	}
}

void BossComponent::SetBossStart(int ID)
{
	// 敵のデータ取得
	switch (ID)
	{
	case 101:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage1Boss);
		break;
	case 102:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage2Boss);
		break;
	case 103:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage3Boss);
		break;
	case 104:
		status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage4Boss);
		break;
	}
	HP = status.HP;

	boss->GetComponent<BossHPBarComponent>()->SetMaxHP(status.HP);
	boss->GetComponent<BossHPBarComponent>()->SetDisplayHP();
}
