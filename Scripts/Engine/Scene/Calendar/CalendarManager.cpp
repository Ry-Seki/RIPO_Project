/*
 *  @file   CalendarManager.cpp
 *  @author Seki
 */

#include "CalendarManager.h"
#include "../DayAction/ActionManager.h"
#include "../Selection/SelectionManager.h"

/*
 *  初期化処理
 */
void CalendarManager::Initialize() {
    // カレンダーシステムの初期化
    calendarSystem->Initialize();
}
/*
 *  更新処理
 */
void CalendarManager::Update(Engine& engine) {
    Day& day = calendarSystem->GetDay();
    if (!isActive) return;

    if (!inputHandle && !IsEndDayAdvance()) {
        if (CheckHitKey(KEY_INPUT_1)) { 
            inputHandle = true; 
            isActive = false;
            day.ActionDungeon(engine); 
            selection->DungeonSelection(engine);
            action->SetOnComplete([this, &day]() { day.AdvanceDay(); });
        }
        //else if (CheckHitKey(KEY_INPUT_2)) { 
        //    inputHandle = true; 
        //    isActive = false;
        //    day.ActionTraining();
        //}
        //else if (CheckHitKey(KEY_INPUT_3)) { 
        //    inputHandle = true;
        //    isActive = false;
        //    day.ActionShop();
        //}
        //else if (CheckHitKey(KEY_INPUT_4)) {
        //    inputHandle = true;
        //    isActive = false;
        //    day.ActionPartTime();
        //}
    }
    // キー離しで再度入力受付
    if (CheckHitKey(KEY_INPUT_1) == 0 && CheckHitKey(KEY_INPUT_2) == 0 && CheckHitKey(KEY_INPUT_3) == 0 && CheckHitKey(KEY_INPUT_4) == 0) {
        inputHandle = false;
    }
}
/*
 *  描画処理
 */
void CalendarManager::Render() {
    if (!isActive)  return;

    auto day = calendarSystem->GetDay();
    DrawFormatString(50, 20, GetColor(255, 255, 255), "elapsedDay : %d", elapsedDay);
    DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Calendar Sample ===");
    DrawFormatString(50, 70, GetColor(255, 255, 0), "Morning Done: %s", day.morningDone ? "Yes" : "No");
    DrawFormatString(50, 90, GetColor(255, 255, 0), "Afternoon Done: %s", day.afternoonDone ? "Yes" : "No");
    DrawFormatString(50, 130, GetColor(0, 255, 0), "1: Dungeon (AM only)");
    DrawFormatString(50, 150, GetColor(0, 255, 0), "2: Training (half day)");
    DrawFormatString(50, 170, GetColor(0, 255, 0), "3: Shop (half day)");
    DrawFormatString(50, 190, GetColor(0, 255, 0), "4: Part-time (half day)");
}
/*
 *  次の日に進行処理
 */
void CalendarManager::NextDay() {
    isActive = true;
    elapsedDay++;
    calendarSystem->AdvanceDay();
}