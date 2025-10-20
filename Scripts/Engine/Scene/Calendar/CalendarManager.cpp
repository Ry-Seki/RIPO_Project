/*
 *  @file   CalendarManager.cpp
 *  @author Seki
 */

#include "CalendarManager.h"

/*
 *  更新処理
 */
void CalendarManager::Update(Engine& engine) {
    auto day = calendarSystem->GetCurrentDay();
    if (!day) return;

    if (!inputHandle) {
        if (CheckHitKey(KEY_INPUT_1)) { day->ActionDungeon(engine); inputHandle = true; }
        else if (CheckHitKey(KEY_INPUT_2)) { day->ActionTraining(); inputHandle = true; }
        else if (CheckHitKey(KEY_INPUT_3)) { day->ActionShop(); inputHandle = true; }
        else if (CheckHitKey(KEY_INPUT_4)) { day->ActionPartTime(); inputHandle = true; }
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
    auto day = calendarSystem->GetCurrentDay();
    if (!day) {
        DrawFormatString(50, 50, GetColor(255, 0, 0), "すべての日が終了しました");
        return;
    }

    DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Calendar Sample ===");
    DrawFormatString(50, 70, GetColor(255, 255, 0), "Morning Done: %s", day->morningDone ? "Yes" : "No");
    DrawFormatString(50, 90, GetColor(255, 255, 0), "Afternoon Done: %s", day->afternoonDone ? "Yes" : "No");
    DrawFormatString(50, 130, GetColor(0, 255, 0), "1: Dungeon (AM only)");
    DrawFormatString(50, 150, GetColor(0, 255, 0), "2: Training (half day)");
    DrawFormatString(50, 170, GetColor(0, 255, 0), "3: Shop (half day)");
    DrawFormatString(50, 190, GetColor(0, 255, 0), "4: Part-time (half day)");
}
/*
 *  次の日に進行処理
 */
void CalendarManager::NextDay() {
    auto day = calendarSystem->GetCurrentDay();
    if (day) day->AdvanceDay();
    calendarSystem->AdvanceDay();
}
/*
 *  一日の行動終了フラグ取得
 */
bool CalendarManager::IsDayComplete() const {
    auto day = calendarSystem->GetCurrentDay();
    return day && day->IsFinished();
}