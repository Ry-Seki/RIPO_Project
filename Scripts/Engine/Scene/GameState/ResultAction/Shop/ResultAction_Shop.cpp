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

/*
 *	@brief	初期化処理
 */
void ResultAction_Shop::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Shop::Setup() {
	inputHandle = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		auto& context = owner->GetOwner()->GetActionContext();
		for (int i = 0, max = context.buyIDList.size(); i < max; i++) {
			PlayerStatusManager::GetInstance().AddPlayerStatus(context.buyIDList[i]);
		}
	});
}
/*
 *	@brief	更新処理
 */
void ResultAction_Shop::Update(float deltaTime) {
	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
		FadeManager::GetInstance().StartFade(fadeOut, [this]() {
			AdvanceDay();
		});
	}
}
/*
 *	@brief	描画処理
 */
void ResultAction_Shop::Render() {
	PlayerStatusData* status = PlayerStatusManager::GetInstance().GetPlayerStatusData();
	DrawFormatString(50, 100, GetColor(0, 255, 0), "HP : %d", status->base.HP);
	DrawFormatString(50, 120, GetColor(0, 255, 0), "Stamina : %d", status->base.stamina);
	DrawFormatString(50, 140, GetColor(0, 255, 0), "Strength : %d", status->base.strength);
	DrawFormatString(50, 160, GetColor(0, 255, 0), "ResistTime : %d", status->base.resistTime);
	DrawFormatString(50, 180, GetColor(255, 255, 255), "次の日に進む->SpaceKey");
}
/*
 *	@brief	片付け処理
 */
void ResultAction_Shop::Teardown() {
	auto& context = owner->GetOwner()->GetActionContext();
	context.buyIDList.clear();
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
