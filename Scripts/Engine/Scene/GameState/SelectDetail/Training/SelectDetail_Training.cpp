/*
 *	@file	SelectDetail_Training.cpp
 *	@author	Seki
 */

#include "SelectDetail_Training.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Training/MenuSelectTraining.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void SelectDetail_Training::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto trainingMenu = menu.GetMenu<MenuSelectTraining>();
	trainingMenu->SetCallback([this](GameEnum::PlayerStatusType type) {
		DecideTrainingType(type);
	});
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Training::Setup() {
	isStart = false;
	inputHandle = false;
	auto& context = owner->GetOwner()->GetActionContext();
	context.statusType = GameEnum::PlayerStatusType::Invalid;
	MenuManager::GetInstance().OpenMenu<MenuSelectTraining>();
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Training::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void SelectDetail_Training::Render() {
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Training::Teardown() {
}
/*
 *	@brief		トレーニング内容決定
 *	@param[in]	GameEnum::PlayerStatusType type
 */
void SelectDetail_Training::DecideTrainingType(GameEnum::PlayerStatusType type) {
	auto& context = owner->GetOwner()->GetActionContext();
	context.statusType = type;
	owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
}
