/*
 *	@file	GameState_InitialTutorial.cpp
 *	@author	Seki
 */

#include "GameState_InitialTutorial.h"
#include "../GameStateMachine.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"

/*
 *	@brief	初期化処理
 */
void GameState_InitialTutorial::Initialize(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void GameState_InitialTutorial::Setup() {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void GameState_InitialTutorial::Update(float deltaTime) {
	if (isStart) {
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			owner->ChageState(GameEnum::GameState::SelectAction);
		});
	}
}
/*
 *	@brief	描画処理
 */
void GameState_InitialTutorial::Render() {
}
/*
 *	@brief	片付け処理
 */
void GameState_InitialTutorial::Teardown() {
}
