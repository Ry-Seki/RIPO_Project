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
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/PartTime/MenuSelectMiniGameLevel.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Load/Audio/LoadAudio.h"

/*
 *	@brief	初期化処理
 */
void SelectDetail_PartTime::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto level = menu.GetMenu<MenuSelectMiniGameLevel>();
	level->SetCallback([this](GameEnum::MiniGameLevel level) {
		DecideMiniGameLevel(level);
	});

}
/*
 *	@brief	準備前処理
 */
void SelectDetail_PartTime::Setup() {
	isStart = false;
	inputHandle = false;
	auto& context = owner->GetOwner()->GetActionContext();
	context.miniGameLevel = GameEnum::MiniGameLevel::Invalid;
	MenuManager::GetInstance().OpenMenu<MenuSelectMiniGameLevel>();
	AudioUtility::ChangeBGM(GameConst::_PARTSELECT_BGM);
	AudioUtility::PlayBGM();


}
/*
 *	@brief	更新処理
 */
void SelectDetail_PartTime::Update(float deltaTime) {
	
}
/*
 *	@brief	描画処理
 */
void SelectDetail_PartTime::Render() {
	
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_PartTime::Teardown() {
	AudioUtility::StopBGM();
}
/*
 *	@brief		ミニゲーム難易度の決定
 *	@param[in]	GameEnum::MiniGameLevel level
 */
void SelectDetail_PartTime::DecideMiniGameLevel(GameEnum::MiniGameLevel level) {
	auto& context = owner->GetOwner()->GetActionContext();
	context.miniGameLevel = level;
	context.prevIncome = MoneyManager::GetInstance().GetCurrentMoney();
	if (level == GameEnum::MiniGameLevel::Invalid) {
		owner->GetOwner()->ChageState(GameEnum::GameState::SelectAction);
	} else {
		owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
	}
}
