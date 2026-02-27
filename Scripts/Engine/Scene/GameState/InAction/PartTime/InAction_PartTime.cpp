/*
 *	@file	InAction_PartTime.cpp
 *	@author	Seki
 */

#include "InAction_PartTime.h"
#include "MiniGame/Sokoban/MiniGameSokoban.h"
#include "../GameState_InAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/PartTime/MenuMiniGame.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Load/Audio/LoadAudio.h"
#include "../../../../Load/LoadManager.h"

/*
 *	@brief	初期化処理
 */
void InAction_PartTime::Initialize(Engine& engine) {
	auto& menu = MenuManager::GetInstance();
	auto miniGame = menu.GetMenu<MenuMiniGame>();
	miniGame->SetCallback([this](GameEnum::MiniGameLevel level) {
		CompleteMiniGame(level);
	});
	auto partBGM = LoadManager::GetInstance().LoadResource<LoadAudio>(_PART_BGMPATH);
	LoadManager::GetInstance().SetOnComplete([&engine, this, partBGM]() {
		AudioUtility::RegisterBGMHandle("partBGM", partBGM->GetHandle());
		});
}
/*
 *	@brief	準備前処理
 */
void InAction_PartTime::Setup() {
	auto& menu = MenuManager::GetInstance();
	auto& context = owner->GetOwner()->GetActionContext();
	auto miniGame = menu.GetMenu<MenuMiniGame>();
	miniGame->SetMiniGameLevel(context.miniGameLevel);
	menu.OpenMenu<MenuMiniGame>();
}
/*
 *	@brief	更新処理
 */
void InAction_PartTime::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void InAction_PartTime::Render() {
}
/*
 *	@brief	片付け処理
 */
void InAction_PartTime::Teardown() {
}
/*
 *	@brief		ミニゲーム完了処理
 *	@param[in]	GameEnum::MiniGameLevel level
 */
void InAction_PartTime::CompleteMiniGame(GameEnum::MiniGameLevel level) {
	auto& context = owner->GetOwner()->GetActionContext();
	context.miniGameLevel = level;
	owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
}
