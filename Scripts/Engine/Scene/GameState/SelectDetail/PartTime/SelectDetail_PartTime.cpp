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
#include "../../../../Load/JSON/LoadJSON.h"

namespace {
	constexpr const char* _SOKOBAN_REWARD_PATH = "Data/MiniGame/Sokoban/SokobanReward.json";
}
/*
 *	@brief	初期化処理
 */
void SelectDetail_PartTime::Initialize() {
	auto& load = LoadManager::GetInstance();
	auto& menu = MenuManager::GetInstance();
	auto reward = load.LoadResource<LoadJSON>(_SOKOBAN_REWARD_PATH);
	auto level = menu.GetMenu<MenuSelectMiniGameLevel>();
	// ロード後のコールバック
	load.SetOnComplete([this, reward, level]() {
		// 報酬データのパース
		ToRewardList(reward->GetData());
		// 報酬データリストをメニュークラスに渡す
		level->SetLevelRewardList(rewardList);
		// 報酬テキストの設定
		level->SetRewardTexts();
	});
	// 難易度決定処理のコールバック登録
	level->SetCallback([this](GameEnum::MiniGameLevel level) {
		DecideMiniGameLevel(level);
	});
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_PartTime::Setup() {
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
 *	@brief		JSON->rewardListに変換
 *	@param[in]	const JSON& json
 */
void SelectDetail_PartTime::ToRewardList(const JSON& json) {
	rewardList.resize(static_cast<int>(GameEnum::MiniGameLevel::Max));

	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Easy)]
		= json.value("Easy", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Normal)]
		= json.value("Normal", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Hard)]
		= json.value("Hard", 0);
	rewardList[static_cast<int>(GameEnum::MiniGameLevel::Retire)]
		= json.value("Retire", 0);
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
