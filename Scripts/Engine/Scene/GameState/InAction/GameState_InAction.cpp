/*
 *	@file	GameState_InAction.cpp
 *	@author	Seki
 */

#include "GameState_InAction.h"
#include "../GameStateMachine.h"
#include "Dungeon/InAction_Dungeon.h"
#include "Training/InAction_Training.h"
#include "Shop/InAction_Shop.h"
#include "PartTime/InAction_PartTime.h"

#include "../../../GameEnum.h"

/*
 *	@brief	初期化処理
 */
void GameState_InAction::Initialize(Engine& engine) {
	// 行動の要素分をあらかじめ確保する
	inActionList.resize(static_cast<int>(GameEnum::ActionType::Max));
	// 各行動クラスを生成
	inActionList[static_cast<int>(GameEnum::ActionType::Dungeon)]
		= std::make_shared<InAction_Dungeon>();
	inActionList[static_cast<int>(GameEnum::ActionType::Training)]
		= std::make_shared<InAction_Training>();
	inActionList[static_cast<int>(GameEnum::ActionType::Shop)]
		= std::make_shared<InAction_Shop>();
	inActionList[static_cast<int>(GameEnum::ActionType::PartTime)]
		= std::make_shared<InAction_PartTime>();
	// 各行動クラスの初期化
	for (auto& inAction : inActionList) {
		if (!inAction) continue;

		inAction->Initialize(engine);
		inAction->SetOwner(this);
	}
}
/*
 *	@brief	準備前処理
 */
void GameState_InAction::Setup() {
	auto& context = owner->GetActionContext();
	currentAction = inActionList[static_cast<int>(context.actionType)];
	currentAction->Setup();
}
/*
 *	@brief	更新処理
 */
void GameState_InAction::Update(float deltaTime) {
	if (currentAction) currentAction->Update(deltaTime);
}
/*
 *	@brief	描画処理
 */
void GameState_InAction::Render() {
	if (currentAction) currentAction->Render();
}
/*
 *	@brief	片付け処理
 */
void GameState_InAction::Teardown() {
	if (currentAction) currentAction->Teardown();
}
/*
 *	@brief	ロード済みデータのセット(コールバック)
 */
void GameState_InAction::SetupData() {
}
