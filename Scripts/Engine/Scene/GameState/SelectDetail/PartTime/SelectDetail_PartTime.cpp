/*
 *	@file	SelectDetail_PartTime.cpp
 *	@author	Seki
 */

#include "SelectDetail_PartTime.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"

/*
 *	@brief	初期化処理
 */
void SelectDetail_PartTime::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_PartTime::Setup() {
	isStart = false;
	inputHandle = false;
	auto& context = owner->GetOwner()->GetActionContext();
	context.miniGameLevel = GameEnum::MiniGameLevel::Invalid;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black , 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void SelectDetail_PartTime::Update(float deltaTime) {
	if (!isStart) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.miniGameLevel = GameEnum::MiniGameLevel::Easy;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	} else if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.miniGameLevel = GameEnum::MiniGameLevel::Normal;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	} else if (!inputHandle && CheckHitKey(KEY_INPUT_3)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.miniGameLevel = GameEnum::MiniGameLevel::Hard;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	}

	if (CheckHitKey(KEY_INPUT_1) && CheckHitKey(KEY_INPUT_2) 
		&& CheckHitKey(KEY_INPUT_3) && CheckHitKey(KEY_INPUT_4) == 0) inputHandle = false;
}
/*
 *	@brief	描画処理
 */
void SelectDetail_PartTime::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Level ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: Easy");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Normal");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: hard");
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_PartTime::Teardown() {
}
