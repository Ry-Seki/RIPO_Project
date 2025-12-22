/*
 *	@file	GameState_SelectAction.cpp
 *	@author	Seki
 */

#include "GameState_SelectAction.h"
#include "../../../Audio/AudioUtility.h"

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
            inputHandle = true;
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
        }
        else if (CheckHitKey(KEY_INPUT_2)) {
            inputHandle = true;
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
        }
        else if (CheckHitKey(KEY_INPUT_3)) {
            inputHandle = true;
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
        }
        else if (CheckHitKey(KEY_INPUT_4)) {
            inputHandle = true;
            // SEの再生
            AudioUtility::PlaySE("DebugSE");
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
