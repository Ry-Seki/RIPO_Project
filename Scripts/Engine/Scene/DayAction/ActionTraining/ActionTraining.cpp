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
 *	初期化処理
 */
void ActionTraining::Initialize(Engine& engine) {
	auto fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		PlayerStatusManager::GetInstance().AddPlayerStatus(actionNum);
	});
}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionTraining::SetupData(Engine& engine) {

}
/*
 *	更新処理
 */
void ActionTraining::Update(Engine& engine, float deltaTime) {
	timer += deltaTime;
	if (timer >= limitTime) {
		isComplete = true;
	}
}
/*
 *	描画処理
 */
void ActionTraining::Render() {
	PlayerStatusData* status = PlayerStatusManager::GetInstance().GetPlayerStatusData();
	DrawFormatString(50, 130, GetColor(0, 255, 0), "HP : %d", status->base.HP);
	DrawFormatString(50, 150, GetColor(0, 255, 0), "Stamina : %d", status->base.stamina);
	DrawFormatString(50, 170, GetColor(0, 255, 0), "Strength : %d", status->base.strength);
	DrawFormatString(50, 190, GetColor(0, 255, 0), "ResistTime : %d", status->base.resistTime);
}
/*
 *	破棄処理
 */
void ActionTraining::Teardown() {
}