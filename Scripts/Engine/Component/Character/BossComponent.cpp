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
	animationHandle = MV1LoadModel("Res/Model/Enemy/Boss/Boss.mv1");
	boss = GetOwner();
	if (boss == nullptr) return;
	boss->GetComponent<AnimatorComponent>()->SetModelHandle(animationHandle);
}

/*
 *	更新処理
 */
void BossComponent::Update(float deltaTime)
{

}