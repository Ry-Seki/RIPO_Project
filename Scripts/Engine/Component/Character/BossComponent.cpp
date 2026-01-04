#include "BossComponent.h"
#include "../AnimatorComponent.h"
#include "../../Manager/GameObjectManager.h"

/*
 *	コンストラクタ
 */
BossComponent::BossComponent()
	: boss(nullptr)
	, animationHandle(-1)
{
}

void BossComponent::Start()
{
	// モデルハンドルの読み込み
	animationHandle = MV1LoadModel("Res/Model/Enemy/Boss/Boss.mv1");
	boss = GetOwner();
	if (boss == nullptr) return;
	auto animator = boss->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// モデルハンドルのセット
	animator->SetModelHandle(animationHandle);
	animator->SetAttachIndex(animationHandle);
}

/*
 *	更新処理
 */
void BossComponent::Update(float deltaTime)
{

}