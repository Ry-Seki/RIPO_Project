/*
 *	@file	ExitPoint.h
 *  @author kuu & oorui
 */

#include "ExitPoint.h"
#include "../../Fade/FadeFactory.h"
#include "../../GameConst.h"
#include "../../System/Money/MoneyManager.h"
#include "../../Component/Character/PlayerComponent.h"
#include "../../Component/Character/ArmActionComponent.h"
#include "../../Stage/StageObject/StageObjectBase.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../Manager/EffectManager.h"

using namespace CharacterUtility;

ExitPoint::ExitPoint()
	: effectName("GoalEffect")
	, viewRadius(5000.0f)
	, exitTriger(false)
	, pViewingEffect(nullptr) {
}

/*
 *	更新
 */
void ExitPoint::Update(float deltaTime) {
	// プレイヤーの取得
	auto player = GetPlayer();
	if (!player)return;

	// プレイヤーと出口の距離
	float distance = Distance(player->position, GetOwner()->position);

	if (distance < viewRadius) {
		if (!pViewingEffect) {
			pViewingEffect = EffectManager::GetInstance().Instantiate(
				effectName,
				GetOwner()->position
			);
		}
	}
	else {
		// エフェクトが存在すれば停止
		if (pViewingEffect) {
			pViewingEffect->EffectStop();
			pViewingEffect = nullptr;
		}

	}



	// エフェクト再生
}

// 出口の衝突イベント
void ExitPoint::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// 既にプレイヤーと衝突していたら処理しない
	if (exitTriger || other->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER)
		return;

	 

	// 衝突済みにする
	exitTriger = true;

	// エフェクト停止
	pViewingEffect->EffectStop();

	auto player = other->GetOwner()->GetComponent<PlayerComponent>();
	if (!player)return;
	auto treasure = player->GetOwner()->GetComponent<ArmActionComponent>()->GetLiftObject();
	if (!treasure)return;
	auto treasureID = treasure->GetComponent<StageObjectBase>();
	if (!treasureID)return;

	MoneyManager::GetInstance().AddTreasureMoney(treasureID->GetTreasureID());
}

// モデルの読み込み
void ExitPoint::ModelLoad(const int modelHandleBase) {

}
