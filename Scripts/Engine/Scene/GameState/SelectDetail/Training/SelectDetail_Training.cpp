/*
 *	@file	SelectDetail_Training.cpp
 *	@author	Seki
 */

#include "SelectDetail_Training.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void SelectDetail_Training::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Training::Setup() {
	isStart = false;
	inputHandle = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Training::Update(float deltaTime) {
	if (!isStart) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.statusType = GameEnum::PlayerStatusType::HP;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.statusType = GameEnum::PlayerStatusType::Stamina;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_3)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.statusType = GameEnum::PlayerStatusType::Strength;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_4)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			auto& context = owner->GetOwner()->GetActionContext();
			context.statusType = GameEnum::PlayerStatusType::ResistTime;
			owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
		});
	}
	if (CheckHitKey(KEY_INPUT_1) && CheckHitKey(KEY_INPUT_2) && CheckHitKey(KEY_INPUT_3) && CheckHitKey(KEY_INPUT_4) == 0) inputHandle = false;
}
/*
 *	@brief	描画処理
 */
void SelectDetail_Training::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Training ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: HP");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Stamina");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: Strength");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "4: ResistTime");
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Training::Teardown() {
	inputHandle = false;
}
