/*
 *	@file	ActionTraining.cpp
 *	@author	Seki
 */

#include "ActionTraining.h"
#include "../../../System/Status/PlayerStatusManager.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ActionTraining::Initialize(Engine& engine) {
	auto fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		PlayerStatusManager::GetInstance().AddPlayerStatus(actionNum);
	});
}
/*
 *	@brief	準備前処理
 */
void ActionTraining::Setup(Engine& engine) {
}
/*
 *  @brief	ロード済みのデータをセット(コールバック)
 */
void ActionTraining::SetupData(Engine& engine) {

}
/*
 *	@brief	更新処理
 */
void ActionTraining::Update(Engine& engine, float deltaTime) {
	timer += deltaTime;
	if (timer >= limitTime) {
		isComplete = true;
	}
}
/*
 *	@brief	描画処理
 */
void ActionTraining::Render() {
	PlayerStatusData* status = PlayerStatusManager::GetInstance().GetPlayerStatusData();
	DrawFormatString(50, 100, GetColor(0, 255, 0), "HP : %d", status->base.HP);
	DrawFormatString(50, 120, GetColor(0, 255, 0), "Stamina : %d", status->base.stamina);
	DrawFormatString(50, 140, GetColor(0, 255, 0), "Strength : %d", status->base.strength);
	DrawFormatString(50, 160, GetColor(0, 255, 0), "ResistTime : %d", status->base.resistTime);
}
/*
 *	@brief	破棄処理
 */
void ActionTraining::Teardown() {
}