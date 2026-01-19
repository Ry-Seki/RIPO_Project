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
#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ResultAction_Training::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Training::Setup() {
	inputHandle = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		auto& context = owner->GetOwner()->GetActionContext();
		PlayerStatusManager::GetInstance().AddPlayerStatus(static_cast<int>(context.statusType));
	});
}
/*
 *	@brief	更新処理
 */
void ResultAction_Training::Update(float deltaTime) {
	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
		FadeManager::GetInstance().StartFade(fadeOut, [this]() {
			owner->GetOwner()->ChageState(GameEnum::GameState::SelectAction);
		});
	}
}
/*
 *	@brief	描画処理
 */
void ResultAction_Training::Render() {
	PlayerStatusData* status = PlayerStatusManager::GetInstance().GetPlayerStatusData();
	DrawFormatString(50, 100, GetColor(0, 255, 0), "HP : %d", status->base.HP);
	DrawFormatString(50, 120, GetColor(0, 255, 0), "Stamina : %d", status->base.stamina);
	DrawFormatString(50, 140, GetColor(0, 255, 0), "Strength : %d", status->base.strength);
	DrawFormatString(50, 160, GetColor(0, 255, 0), "ResistTime : %d", status->base.resistTime);
}
/*
 *	@brief	片付け処理
 */
void ResultAction_Training::Teardown() {
}
