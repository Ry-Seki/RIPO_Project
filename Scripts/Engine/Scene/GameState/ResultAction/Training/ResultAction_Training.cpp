/*
 *	@file	ResultAction_Training.cpp
 *	@author	Seki
 */

#include "ResultAction_Training.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../System/Status/PlayerStatusManager.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Status/MenuPlayerStatus.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ResultAction_Training::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto status = menu.GetMenu<MenuPlayerStatus>();
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Training::Setup() {
	auto& player = PlayerStatusManager::GetInstance();
	auto& menu = MenuManager::GetInstance();
	auto status = menu.GetMenu<MenuPlayerStatus>();
	status->SetIsCallback(true);
	status->SetPrevStatusData(player.GetPlayerStatusData());
	status->SetCallback([this]() {
		AdvanceDay();
	});
	auto& context = owner->GetOwner()->GetActionContext();
	player.AddPlayerStatus(static_cast<int>(context.statusType));
	menu.OpenMenu<MenuPlayerStatus>();
}
/*
 *	@brief	更新処理
 */
void ResultAction_Training::Update(float deltaTime) {
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
void ResultAction_Training::Render() {

}
/*
 *	@brief	片付け処理
 */
void ResultAction_Training::Teardown() {
}
/*
 *	@brief	日にち進行処理
 */
void ResultAction_Training::AdvanceDay() {
	auto& context = owner->GetOwner()->GetActionContext();
	// 半日をすでに消化しているかで分岐
	if (context.isHalf) {
		// 一日進める
		context.isHalf = false;
		context.elapsedDay++;
	}
	else {
		// 半日進める
		context.isHalf = true;
	}
	// アクション終了フラグの変更
	owner->GetOwner()->SetIsActionEnd(true);
}
