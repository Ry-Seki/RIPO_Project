/*
 *	@file	BossComponent.cpp
 *	@author	kuu
 */
#include "BossComponent.h"
#include "../../Manager/GameObjectManager.h"
#include "BossStandby.h"

/*
 *	コンストラクタ
 */
BossComponent::BossComponent()
	: BossComponent(new BossStandby())
{
}

BossComponent::BossComponent(BossState* initState)
	: boss(nullptr)
	//, modelRenderer(nullptr)
	, animator(nullptr)
	, state(initState)
	, animationHandle(-1)
	, modelHandle(-1)
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

}