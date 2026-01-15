/*
 *	@file	GameState_ResultAction.cpp
 *	@author	Seki
 */

#include "GameState_ResultAction.h"
#include "Dungeon/ResultAction_Dungeon.h"
#include "Training/ResultAction_Training.h"
#include "Shop/ResultAction_Shop.h"
#include "PartTime/ResultAction_PartTime.h"

/*
 *	@brief	初期化処理
 */
void GameState_ResultAction::Initialize(Engine& engine) {
	// 行動の要素分をあらかじめ生成する
	resultActionList.resize(static_cast<int>(GameEnum::ActionType::Max));
	resultActionList[static_cast<int>(GameEnum::ActionType::Dungeon)]
		= std::make_shared<ResultAction_Dungeon>();
	resultActionList[static_cast<int>(GameEnum::ActionType::Training)]
		= std::make_shared<ResultAction_Training>();
	resultActionList[static_cast<int>(GameEnum::ActionType::Shop)]
		= std::make_shared<ResultAction_Shop>();
	resultActionList[static_cast<int>(GameEnum::ActionType::PartTime)]
		= std::make_shared<ResultAction_PartTime>();

	// 各結果クラスの初期化
	for (auto& result : resultActionList) {
		if (!result) return;

		result->Initialize();
		result->SetOnwer(this);
	}
}
/*
 *	@brief	準備前処理
 */
void GameState_ResultAction::Setup() {
}
/*
 *	@brief	更新処理
 */
void GameState_ResultAction::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void GameState_ResultAction::Render() {
}
/*
 *	@brief	片付け処理
 */
void GameState_ResultAction::Teardown() {

}
