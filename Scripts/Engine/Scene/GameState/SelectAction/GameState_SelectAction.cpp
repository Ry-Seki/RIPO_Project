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
#include "../../../Menu/MainGame/Status/MenuPlayerStatus.h"
#include "../../../System/Status/PlayerStatusManager.h"

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
    auto& context = owner->GetActionContext();
    auto& menu = MenuManager::GetInstance();
    context.actionType = GameEnum::ActionType::Invalid;
    auto selectMenu = menu.GetMenu<MenuSelectAction>();
    selectMenu->SetElapsedDay(context.elapsedDay);
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
    auto& menu = MenuManager::GetInstance();
    // Maxの場合はステータス画面を開く 
    if (type == GameEnum::ActionType::Max) {
        auto status = menu.GetMenu<MenuPlayerStatus>();
        status->SetIsCallback(false);
        status->SetPrevStatusData(PlayerStatusManager::GetInstance().GetPlayerStatusData());
        menu.OpenMenu<MenuPlayerStatus>();
    } else {
        auto& context = owner->GetActionContext();
        context.actionType = type;
        AudioUtility::StopBGM();
        menu.CloseTopMenu();        // アクション選択メニュー
        owner->ChageState(GameEnum::GameState::SelectDetail);
    }
}
