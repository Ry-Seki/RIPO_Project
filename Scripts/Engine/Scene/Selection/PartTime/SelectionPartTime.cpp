/*
 *	@file	SelectionPartTime.cpp
 *	@author	Seki
 */

#include "SelectionPartTime.h"
#include "../SelectionFactory.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"
#include "MiniGame/Sokoban/MiniGameSokoban.h"

/*
 *	@brief	初期化処理
 */
void SelectionPartTime::Initialize(Engine& engine) {
	miniGameList.push_back(std::make_shared<MiniGameSokoban>());
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [&engine, this]() {
		SetupData(engine);
	});
}
/*
 *	@brief	準備前処理
 */
void SelectionPartTime::Setup(Engine& engine) {
}
/*
 *	@brief	ロード済みデータのセット(コールバック)
 */
void SelectionPartTime::SetupData(Engine& engine) {
	currentMiniGame = miniGameList[0];
	currentMiniGame->Open();
}
/*
 *	@brief	更新処理
 */
void SelectionPartTime::Update(Engine& engine, float deltaTime) {
	// ミニゲームの更新
	currentMiniGame->Update(engine, deltaTime);

	// ミニゲーム終了時、その結果を反映
	if (currentMiniGame->IsComplete()) {
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActivePartTime(engine, currentMiniGame->GetScore());
		});
	}
}
/*
 *	@brief	描画処理
 */
void SelectionPartTime::Render() {
	if (!currentMiniGame) return;
	currentMiniGame->Render();
}
/*
 *	@brief	片付け処理
 */
void SelectionPartTime::Teardown() {
	partTimeScore = 0;
}
/*
 *  @brief  行動実行関数の呼び出し
 */
void SelectionPartTime::SetAction() {
}
