/*
 *  @file   CalendarManager.cpp
 *  @author Seki
 */

#include "CalendarManager.h"

void CalendarManager::StartDayAdvanceWithFade() {
    if (isFading) return;
    isFading = true;

    // フェードアウト開始
    FadeManager::GetInstance().StartFade(
        CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop),
        [this]() {
        // フェードアウト完了 → 日進行
        calendarSystem->AdvanceDay();

        // フェードイン開始
        FadeManager::GetInstance().StartFade(
            CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop),
            [this]() {
            // フェードイン完了 → 入力受付再開
            isFading = false;
            inputHandle = false;
        }
        );
    }
    );
}

void CalendarManager::Update() {
    auto day = calendarSystem->GetCurrentDay();
    if (!day || isFading) return; // フェード中は入力不可

    // キー入力処理（1フレームで1回のみ）
    if (!inputHandle) {
        if (CheckHitKey(KEY_INPUT_1)) { day->ActionDungeon(); inputHandle = true; StartDayAdvanceWithFade(); }
        else if (CheckHitKey(KEY_INPUT_2)) { day->ActionTraining(); inputHandle = true; StartDayAdvanceWithFade(); }
        else if (CheckHitKey(KEY_INPUT_3)) { day->ActionShop(); inputHandle = true; StartDayAdvanceWithFade(); }
        else if (CheckHitKey(KEY_INPUT_4)) { day->ActionPartTime(); inputHandle = true; StartDayAdvanceWithFade(); }
    }

    // 入力が離されたら再度フラグ解除（念のため）
    if (CheckHitKey(KEY_INPUT_1) == 0 &&
        CheckHitKey(KEY_INPUT_2) == 0 &&
        CheckHitKey(KEY_INPUT_3) == 0 &&
        CheckHitKey(KEY_INPUT_4) == 0) {
        inputHandle = false;
    }
}

void CalendarManager::Render() {
    auto day = calendarSystem->GetCurrentDay();
    if (!day) {
        DrawFormatString(50, 50, GetColor(255, 0, 0), "すべての日が終了しました");
        return;
    }

    // 日付・状態表示
    DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Calendar Sample ===");
    DrawFormatString(50, 70, GetColor(255, 255, 0), "Morning Done: %s", day->morningDone ? "Yes" : "No");
    DrawFormatString(50, 90, GetColor(255, 255, 0), "Afternoon Done: %s", day->afternoonDone ? "Yes" : "No");

    // 行動選択表示
    DrawFormatString(50, 130, GetColor(0, 255, 0), "1: Dungeon (AM only)");
    DrawFormatString(50, 150, GetColor(0, 255, 0), "2: Training (half day)");
    DrawFormatString(50, 170, GetColor(0, 255, 0), "3: Shop (half day)");
    DrawFormatString(50, 190, GetColor(0, 255, 0), "4: Part-time (half day)");
}