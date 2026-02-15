/*
 *	@file	ResultAction_Shop.cpp
 *	@author	Seki
 */

#include "ResultAction_Shop.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../System/Status/PlayerStatusManager.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Status/MenuPlayerStatus.h"

/*
 *	@brief	初期化処理
 */
void ResultAction_Shop::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto status = menu.GetMenu<MenuPlayerStatus>();
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Shop::Setup() {
	auto& player = PlayerStatusManager::GetInstance();
	auto& menu = MenuManager::GetInstance();
	auto status = menu.GetMenu<MenuPlayerStatus>();
	status->SetIsCallback(true);
	status->SetPrevStatusData(player.GetPlayerStatusData());
	status->SetCallback([this]() {
		AdvanceDay();
	});
	auto& context = owner->GetOwner()->GetActionContext();
	for (int i = 0, max = context.buyStatusIDList.size(); i < max; i++) {
		PlayerStatusManager::GetInstance().AddPlayerStatus(context.buyStatusIDList[i]);
	}
	menu.OpenMenu<MenuPlayerStatus>();
}
/*
 *	@brief	更新処理
 */
void ResultAction_Shop::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void ResultAction_Shop::Render() {
}
/*
 *	@brief	片付け処理
 */
void ResultAction_Shop::Teardown() {
	auto& context = owner->GetOwner()->GetActionContext();
	context.buyStatusIDList.clear();
	context.buyWeaponIDList.clear();
}
/*
 *	@brief	日にち進行処理
 */
void ResultAction_Shop::AdvanceDay() {
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
