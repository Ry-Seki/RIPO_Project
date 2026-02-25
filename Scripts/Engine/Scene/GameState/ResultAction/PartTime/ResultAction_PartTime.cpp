/*
 *	@file	ResultAction_PartTime.cpp
 *	@author	Seki
 */

#include "ResultAction_PartTime.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../Load/JSON/LoadJSON.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Money/MenuMoneyChange.h"

/*
 *	@brief	初期化処理
 */
void ResultAction_PartTime::Initialize() {
	auto& load = LoadManager::GetInstance();
	auto& menu = MenuManager::GetInstance();
	auto money = menu.GetMenu<MenuMoneyChange>();
	auto reward = load.LoadResource<LoadJSON>(_SOKOBAN_REWARD_PATH);
	load.SetOnComplete([this, reward]() {
		ToRewardList(reward->GetData());
	});
	money->SetCallback([this]() {
		AdvanceDay();
	});
}
/*
 *	@brief	準備前処理
 */
void ResultAction_PartTime::Setup() {
	auto& menu = MenuManager::GetInstance();
	auto& context = owner->GetOwner()->GetActionContext();
	int reward = static_cast<int>(context.miniGameLevel);
	MoneyManager::GetInstance().AddMoney(rewardList[reward]);
	auto money = menu.GetMenu<MenuMoneyChange>();
	money->SetCurrentMoney(MoneyManager::GetInstance().GetCurrentMoney());
	money->SetPrevMoney(context.prevIncome);
	menu.OpenMenu<MenuMoneyChange>();
}
/*
 *	@brief	更新処理
 */
void ResultAction_PartTime::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void ResultAction_PartTime::Render() {
}
/*
 *	@brief	片付け処理
 */
void ResultAction_PartTime::Teardown() {
}
/*
 *	@brief	日にち進行処理
 */
void ResultAction_PartTime::AdvanceDay() {
	auto& context = owner->GetOwner()->GetActionContext();
	// 半日をすでに消化しているかで分岐
	if (context.isHalf) {
		// 一日進める
		context.isHalf = false;
		context.elapsedDay++;
	} else {
		// 半日進める
		context.isHalf = true;
	}
	// アクション終了フラグの変更
	owner->GetOwner()->SetIsActionEnd(true);
}
/*
 *	@brief		JSON->rewardListに変換
 *	@param[in]	const JSON& json
 */
void ResultAction_PartTime::ToRewardList(const JSON& json) {
	rewardList.resize(static_cast<int>(GameEnum::MiniGameLevel::Max));

	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Easy)]
		= json.value("Easy", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Normal)]
		= json.value("Normal", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Hard)]
		= json.value("Hard", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Retire)] 
		= json.value("Retire", 0);
}
