/*
 *	@file	ResultAction_Dungeon.cpp
 *	@author	Seki
 */

#include "ResultAction_Dungeon.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Dungeon/MenuPlayerDeath.h"
#include "../../../../Menu/MainGame/Money/MenuMoneyChange.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ResultAction_Dungeon::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto death = menu.GetMenu<MenuPlayerDeath>();
	auto money = menu.GetMenu<MenuMoneyChange>();
	death->SetCallback([this]() {
		AdvanceDay();
	});
	money->SetCallback([this]() {
		AdvanceDay();
	});
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Dungeon::Setup() {
	inputHandle = false;
	isStart = false;
	ResultPlayerAction();
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void ResultAction_Dungeon::Update(float deltaTime) {
	if (!isStart) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		AdvanceDay();
	}
}
/*
 *	@brief	描画処理
 */
void ResultAction_Dungeon::Render() {
	if (!isStart) return;

}
/*
 *	@brief	片付け処理
 */
void ResultAction_Dungeon::Teardown() {
	inputHandle = false;
}
/*
 *	@brief	日にち進行処理
 */
void ResultAction_Dungeon::AdvanceDay() {
	// 一日進める
	auto& context = owner->GetOwner()->GetActionContext();
	context.elapsedDay++;
	context.isHalf = false;
	// アクション終了フラグの変更
	owner->GetOwner()->SetIsActionEnd(true);
}
/*
 *	@brief	プレイヤーのリザルト処理
 */
void ResultAction_Dungeon::ResultPlayerAction() {
	auto& menu = MenuManager::GetInstance();
	auto& context = owner->GetOwner()->GetActionContext();

	if (context.isPlayerDead) {
		menu.OpenMenu<MenuPlayerDeath>();
	} else {
		auto money = menu.GetMenu<MenuMoneyChange>();
		money->SetCurrentMoney(MoneyManager::GetInstance().GetCurrentMoney());
		money->SetPrevMoney(context.prevIncome);
		menu.OpenMenu<MenuMoneyChange>();
	}
}
