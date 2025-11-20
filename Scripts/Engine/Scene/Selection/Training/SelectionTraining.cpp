/*
 *	@file	SelectionTraining.cpp
 *	@author	Seki
 */

#include "SelectionTraining.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"

using namespace AudioUtility;

/*
 *	@brief	初期化処理
 */
void SelectionTraining::Initialize(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	ロード済みデータのセット
 */
void SelectionTraining::SetupData(Engine& engine) {
}
/*
 *	更新処理
 */
void SelectionTraining::Update(Engine& engine, float deltaTime) {
	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActiveTraining(engine, 0);
		});
	} else if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActiveTraining(engine, 1);
		});
	} else if (!inputHandle && CheckHitKey(KEY_INPUT_3)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActiveTraining(engine, 2);
		});
	} else if (!inputHandle && CheckHitKey(KEY_INPUT_4)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActiveTraining(engine, 3);
		});
	}

	if (CheckHitKey(KEY_INPUT_1) && CheckHitKey(KEY_INPUT_2) && CheckHitKey(KEY_INPUT_3) && CheckHitKey(KEY_INPUT_4)== 0) inputHandle = false;
}
/*
 *	描画処理
 */
void SelectionTraining::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Training Sample ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: HP");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Stamina");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: Strength");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "4: ResistTime");
}
