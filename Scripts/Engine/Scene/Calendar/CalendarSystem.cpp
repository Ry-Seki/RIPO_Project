/*
 *  @file   CalendarSystem.cpp
 *  @author Seki
 */

#include "CalendarSystem.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"

/*
 *  初期化処理
 */
void CalendarSystem::Initialize() {
    // Yearクラスの初期化
    year = std::make_unique<Year>();
    // セーブデータの受け取り

    // 経過日数の設定

}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void CalendarSystem::Setup() {

}
/*
 *  日にちの進行
 */
void CalendarSystem::AdvanceDay() {
    if (GetMonth().IsFinished()) return;
    // 1年が終了したら処理しない
    //if (GetYear().IsFinished()) return;

    // 1日の消費
    GetDay().AdvanceDay();
    // Weekクラスの進行処理
    if (GetDay().IsFinished()) GetWeek().Advance();
    // Monthクラスの進行処理
    if (GetWeek().IsFinished()) GetMonth().Advance();
    // Yearクラスの進行処理
    if (GetMonth().IsFinished()) GetYear().Advance();
}