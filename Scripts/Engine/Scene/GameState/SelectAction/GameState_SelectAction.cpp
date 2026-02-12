/*
 *	@file	GameState_SelectAction.cpp
 *	@author	Seki
 */

#include "GameState_SelectAction.h"
#include "../../../Audio/AudioUtility.h"
#include "../GameStateMachine.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Input/InputUtility.h"
#include "../../../Menu/MenuManager.h"
#include "../../../Menu/MainGame/MenuInGame.h"
#include "../../../Menu/MainGame/MenuSelectAction.h"

#include <DxLib.h>

/*
 *  @brief  初期化処理
 */
void GameState_SelectAction::Initialize(Engine& engine) {
    auto selectMenu = MenuManager::GetInstance().GetMenu<MenuSelectAction>();
    selectMenu->SetCallback([this](GameEnum::ActionType type) {
        DecideActionType(type);
    });
}
/*
 *	@brief	準備前処理
 */
void GameState_SelectAction::Setup() {
    inputHandle = false;
    auto& context = owner->GetActionContext();
    context.actionType = GameEnum::ActionType::Invalid;
    auto& menu = MenuManager::GetInstance();
    auto selectMenu = menu.GetMenu<MenuSelectAction>();
    selectMenu->SetIsHalf(context.isHalf);
    menu.OpenMenu<MenuSelectAction>();
}
/*
 *	@brief	更新処理
 */
void GameState_SelectAction::Update(float deltaTime) {

}
/*
 *  @brief  描画処理
 */
void GameState_SelectAction::Render() {
}
/*
 *	@brief	片付け処理
 */
void GameState_SelectAction::Teardown() {
    inputHandle = false;
}
/*
 *	@brief		アクションの決定
 *	@param[in]	GameEnum::ActionType type
 */
void GameState_SelectAction::DecideActionType(GameEnum::ActionType type) {
    auto& context = owner->GetActionContext();
    context.actionType = type;
    owner->ChageState(GameEnum::GameState::SelectDetail);
}
