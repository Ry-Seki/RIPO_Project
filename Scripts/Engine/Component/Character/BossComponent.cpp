/*
 *	@file	BossComponent.cpp
 *	@author	kuu
 */
#include "BossComponent.h"
#include "../../Manager/GameObjectManager.h"
#include "BossStandby.h"
#include "../../Manager/CameraManager.h"

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
	//, modelRenderer(nullptr)
	, animator(nullptr)
	, state(initState)
	, animationHandle(-1)
	, modelHandle(-1)
	, coolTime(3)
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
	//animator->SetAttachIndex(animationHandle);

	animator->LoadIndex(true);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	if (state == nullptr)
		state = new BossStandby();
	state->Start(boss);
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
	if (!isTriger && other->GetOwner()->name == "Player") {
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		// 今はとりあえず適当なダメージ
		playerStatus.HP = playerStatus.HP - 10;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		isTriger = true;
	}
	else if (coolTime <= 0) {
		isTriger = false;
		coolTime = 2;
	}
}
