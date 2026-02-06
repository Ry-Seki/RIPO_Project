/*
 *	@file	ResultScene.cpp
 *	@author	Seki
 */

#include "ResultScene.h"
#include "TitleScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../System/Money/MoneyManager.h"
#include "../Load/LoadManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../Menu/MenuManager.h"
#include "../Menu/Result/MenuResultScore.h"

#include <DxLib.h>	

/*
 *	@brief	初期化処理
 */
void ResultScene::Initialize(Engine& engine) {
	MenuManager::GetInstance().GetMenu<MenuResultScore>();
	auto& load = LoadManager::GetInstance();
	auto resultScore = load.LoadResource<LoadJSON>(_RESULT_SCORE_PATH);
	load.SetOnComplete([this, &engine]() {
		Setup(engine);
	});
}
/*
 *	@brief	ロード済みデータのセット (コールバック)
 */
void ResultScene::SetupData(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void ResultScene::Setup(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
		MenuManager::GetInstance().OpenMenu<MenuResultScore>();
	});
}
/*
 *	@brief	更新処理
 */
void ResultScene::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	
}
/*
 *	@brief	描画処理
 */
void ResultScene::Render() {
	MoneyManager& money = MoneyManager::GetInstance();
	DrawFormatString(50, 50, GetColor(255, 255, 255), "ResultMoney : %d", money.GetCurrentMoney());
	DrawFormatString(50, 70, GetColor(255, 255, 255), "ResultScore ");
}
