/*
 *	@file	SelectionPartTime.cpp
 *	@author	Seki
 */

#include "SelectionPartTime.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"

using namespace AudioUtility;
/*
 *	@brief	初期化処理
 */
void SelectionPartTime::Initialize(Engine& engine) {
}
/*
 *	@brief	ロード済みデータのセット(コールバック)
 */
void SelectionPartTime::SetupData(Engine& engine) {
}
/*
 *	@brief	更新処理
 */
void SelectionPartTime::Update(Engine& engine, float deltaTime) {
	// ミニゲーム開始

	// ミニゲーム終了時、その結果を反映
	PlaySE("DebugSE");
	inputHandle = true;
	isComplete = true;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
		ActivePartTime(engine, 25);
	});
}
/*
 *	@brief	描画処理
 */
void SelectionPartTime::Render() {
}
/*
 *	@brief	片付け処理
 */
void SelectionPartTime::Teardown() {
	partTimeScore = 0;
}
