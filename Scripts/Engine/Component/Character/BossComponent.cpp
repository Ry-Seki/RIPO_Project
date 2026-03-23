/*
 *	@file	BossComponent.cpp
 *	@author	kuu
 */
#include "BossComponent.h"
#include "../../Manager/GameObjectManager.h"
#include "BossStandby.h"
#include "PlayerComponent.h"
#include "../HPComponent.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/StageManager.h"
#include "BossDeath.h"
#include "BossChase.h"
#include "BossAttack.h"
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
	, randomCoolTime(0)
	, attackIsTriger(false)
	, damageIsTriger(false)
	, moveFrag(false)
	, hitFlag(false)
	, homePosition(Vector3::zero)
	, moveVec(Vector3::zero)
{
}

BossComponent::~BossComponent()
{
	delete state;
}

void BossComponent::Start()
{
	// モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/Boss/Stage1Boss.mv1");
	boss = GetOwner();
	if (boss == nullptr) return;
	animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	if (state == nullptr)
		state = new BossStandby();
	state->Start(boss);

	animator->LoadIndex(true);

	// 効果音の読み込み
	auto bossAttackSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossAttackSE.mp3");
	auto bossShootActiveSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossShootActiveSE.mp3");
	auto bossShootAttackSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossShootAttackSE.mp3");
	auto bossWalkSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/EnemySE/BossWalkSE.mp3");
	LoadManager::GetInstance().SetOnComplete([this, bossAttackSE, bossShootActiveSE, bossShootAttackSE, bossWalkSE]() {
		AudioUtility::RegisterSEHandle("bossAttackSE", bossAttackSE->GetHandle());
		AudioUtility::RegisterSEHandle("bossShootActiveSE", bossShootActiveSE->GetHandle());
		AudioUtility::RegisterSEHandle("bossShootAttackSE", bossShootAttackSE->GetHandle());
		AudioUtility::RegisterSEHandle("bossWalkSE", bossWalkSE->GetHandle());
		});
}

/*
 *	更新処理
 */
void BossComponent::Update(float deltaTime)
{
	// 移動量を初期化
	moveVec = Vector3::zero;

	if (state == nullptr || boss == nullptr) return;
	state->Update(boss, deltaTime);

	coolTime -= deltaTime;
	// ランダム挙動を細かくするための二重乱数
	auto randMax = GetRand(5);
	auto randTime = GetRand(randMax * 1000000);
	randTime = randTime / 1000000;
	randomCoolTime += randTime;

	if (homePosition == Vector3::zero) {
		homePosition = boss->position;
	}

	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(boss, moveVec);
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
		if (bossAttackTimeFlag) {
			// エフェクトを出す
			EffectManager::GetInstance().Instantiate("AllEnemyHitEffect", other->GetOwner()->position);
			// 当たったらダメージを与える
			player->GetComponent<HPComponent>()->AddDamage(status.attack);
			
			attackIsTriger = true;
		}
	}
	if (other->GetOwner()->name == "Player") {
		coolTime = 1;
	}

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
