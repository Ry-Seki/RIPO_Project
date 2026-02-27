/*
 *	@file	InAction_Training.cpp
 */

#include "InAction_Training.h"
#include "../GameState_InAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Load/Audio/LoadAudio.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Audio/AudioUtility.h"

/*
 *	@brief	初期化処理
 */
void InAction_Training::Initialize(Engine& engine) {
	auto trainingBGM01 = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/BGM/Training/TrainingBGM.mp3");
	LoadManager::GetInstance().SetOnComplete([&engine, this, trainingBGM01]() {
		AudioUtility::RegisterBGMHandle("trainingBGM", trainingBGM01->GetHandle());
	});
}
/*
 *	@brief	準備前処理
 */
void InAction_Training::Setup() {
	isStart = false;
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});

}
/*
 *	@brief	更新処理
 */
void InAction_Training::Update(float deltaTime) {
	if (isStart) {
		isStart = false;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this]() {
			owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
		});
	}
}
/*
 *	@brief	描画処理
 */
void InAction_Training::Render() {
}
/*
 *	@brief	片付け処理
 */
void InAction_Training::Teardown() {
}
