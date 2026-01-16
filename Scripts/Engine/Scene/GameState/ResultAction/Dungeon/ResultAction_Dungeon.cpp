/*
 *	@file	ResultAction_Dungeon.cpp
 *	@author	Seki
 */

#include "ResultAction_Dungeon.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../Audio/AudioUtility.h"
#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ResultAction_Dungeon::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Dungeon::Setup() {
	prevMoney = MoneyManager::GetInstance().GetPrevMoney();
	currentMoney = MoneyManager::GetInstance().GetCurrentMoney();
}
/*
 *	@brief	更新処理
 */
void ResultAction_Dungeon::Update(float deltaTime) {
	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		owner->GetOwner()->ChageState(GameEnum::GameState::SelectAction);
	}
}
/*
 *	@brief	描画処理
 */
void ResultAction_Dungeon::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "前 : %d", prevMoney);
	DrawFormatString(70, 70, GetColor(255, 255, 255), "↓  %d", currentMoney - prevMoney);
	DrawFormatString(50, 90, GetColor(255, 255, 255), "現在 : %d", currentMoney);
	DrawFormatString(50, 120, GetColor(255, 255, 255), "次の日に進む->SpaceKey");
}
/*
 *	@brief	片付け処理
 */
void ResultAction_Dungeon::Teardown() {
	inputHandle = false;
}
