/*
 *	@file	GameState_SelectAction.cpp
 *	@author	Seki
 */

#include "GameState_SelectAction.h"
#include "../../../Audio/AudioUtility.h"
#include "../GameStateMachine.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	準備前処理
 */
void GameState_SelectAction::Setup() {
    inputHandle = false;
}
/*
 *	@brief	更新処理
 */
void GameState_SelectAction::Update(float deltaTime) {
    // TODO : 現在は決め打ちのため、ここをメニューの更新にする
    if (!inputHandle) {
        if (CheckHitKey(KEY_INPUT_1)) {
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
            inputHandle = true;
            owner->SetActionType(GameEnum::ActionType::Dungeon);
            FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fade, [this]() {
                owner->ChageState(GameEnum::GameState::SelectDetail);
            });
        }
        else if (CheckHitKey(KEY_INPUT_2)) {
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
            inputHandle = true;
            owner->SetActionType(GameEnum::ActionType::Training);
            FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fade, [this]() {
                owner->ChageState(GameEnum::GameState::SelectDetail);
            });
        }
        else if (CheckHitKey(KEY_INPUT_3)) {
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
            inputHandle = true;
            owner->SetActionType(GameEnum::ActionType::Shop);
            FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fade, [this]() {
                owner->ChageState(GameEnum::GameState::SelectDetail);
            });
        }
        else if (CheckHitKey(KEY_INPUT_4)) {
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
            inputHandle = true;
            owner->SetActionType(GameEnum::ActionType::PartTime);
            FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fade, [this]() {
                owner->ChageState(GameEnum::GameState::SelectDetail);
            });
        }
    }
}
/*
 *  @brief  描画処理
 */
void GameState_SelectAction::Render() {
    //DrawFormatString(50, 20, GetColor(255, 255, 255), "elapsedDay : %d", elapsedDay);
    DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Calendar Sample ===");
    //DrawFormatString(50, 70, GetColor(255, 255, 0), "Morning Done: %s", day.morningDone ? "Yes" : "No");
    //DrawFormatString(50, 90, GetColor(255, 255, 0), "Afternoon Done: %s", day.afternoonDone ? "Yes" : "No");
    DrawFormatString(50, 130, GetColor(0, 255, 0), "1: Dungeon (AM only)");
    DrawFormatString(50, 150, GetColor(0, 255, 0), "2: Training (half day)");
    DrawFormatString(50, 170, GetColor(0, 255, 0), "3: Shop (half day)");
    DrawFormatString(50, 190, GetColor(0, 255, 0), "4: Part-time (half day)");

}
/*
 *	@brief	片付け処理
 */
void GameState_SelectAction::Teardown() {
    inputHandle = false;
}
