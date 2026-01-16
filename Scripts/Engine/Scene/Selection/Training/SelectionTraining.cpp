/*
 *	@file	SelectionTraining.cpp
 *	@author	Seki
 */

#include "SelectionTraining.h"
#include "../SelectionFactory.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"

/*
 *	@brief	初期化処理
 */
void SelectionTraining::Initialize(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	準備前処理
 */
void SelectionTraining::Setup(Engine& engine) {
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
}
/*
 *	描画処理
 */
void SelectionTraining::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Training Sample ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: HP");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Stamina");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: Strength");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "4: ResistTime");
}
/*
 *  @brief  行動実行関数の呼び出し
 */
void SelectionTraining::SetAction() {
}
