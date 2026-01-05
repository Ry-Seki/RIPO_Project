#include "BossComponent.h"
#include "../AnimatorComponent.h"
#include "../../Manager/GameObjectManager.h"

/*
 *	コンストラクタ
 */
BossComponent::BossComponent()
	: boss(nullptr)
	, animationHandle(-1)
	, modelHandle(-1)
{
}

void BossComponent::Start()
{
	animationHandle = MV1LoadModel("Res/Model/Enemy/Boss/BossAnimation.mv1");
	// モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/Boss/Boss.mv1");
	boss = GetOwner();
	if (boss == nullptr) return;
	auto animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);
	animator->SetAttachIndex(animationHandle);
}

/*
 *	更新処理
 */
void BossComponent::Update(float deltaTime)
{

}