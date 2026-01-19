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

/*
 *	@brief	初期化処理
 */
void ResultAction_PartTime::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void ResultAction_PartTime::Setup() {
	inputHandle = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		auto& context = owner->GetOwner()->GetActionContext();
		// TODO:ここは難易度によって変動させる
		MoneyManager::GetInstance().AddMoney(_INCOME);
	});
}
/*
 *	@brief	更新処理
 */
void ResultAction_PartTime::Update(float deltaTime) {
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
void ResultAction_PartTime::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "GameClear!");
	DrawFormatString(50, 180, GetColor(255, 255, 255), "次の日に進む->SpaceKey");
}
/*
 *	@brief	片付け処理
 */
void ResultAction_PartTime::Teardown() {
}
