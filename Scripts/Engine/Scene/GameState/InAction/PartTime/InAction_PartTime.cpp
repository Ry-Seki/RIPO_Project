/*
 *	@file	InAction_PartTime.cpp
 *	@author	Seki
 */

#include "InAction_PartTime.h"
#include "MiniGame/Sokoban/MiniGameSokoban.h"
#include "../GameState_InAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"

/*
 *	@brief	初期化処理
 */
void InAction_PartTime::Initialize(Engine& engine) {
	miniGameList.push_back(std::make_shared<MiniGameSokoban>());
}
/*
 *	@brief	準備前処理
 */
void InAction_PartTime::Setup() {
	isStart = false;
	// TODO : 難易度によってミニゲーム内容を変動させる
	miniGame = miniGameList[0];
	miniGame->Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void InAction_PartTime::Update(float deltaTime) {
	if (!isStart) return;
	// ミニゲームの更新
	miniGame->Update(deltaTime);

	// ミニゲーム終了時、その結果を反映
	if (miniGame->IsComplete()) {
		isStart = false;
		AudioUtility::PlaySE("DebugSE");
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
		});
	}
}
/*
 *	@brief	描画処理
 */
void InAction_PartTime::Render() {
	if (miniGame) miniGame->Render();
}
/*
 *	@brief	片付け処理
 */
void InAction_PartTime::Teardown() {
}
