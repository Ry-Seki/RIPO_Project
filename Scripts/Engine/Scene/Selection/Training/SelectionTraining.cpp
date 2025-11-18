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
	if (!inputHandle && CheckHitKey(KEY_INPUT_0)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			
		});
	}
	if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	描画処理
 */
void SelectionTraining::Render() {
}
