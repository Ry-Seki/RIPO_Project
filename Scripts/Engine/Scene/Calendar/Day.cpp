/*
 *  @file   Day.cpp
 *  @author Seki
 */

#include "Day.h"
#include "../../Engine.h"
/*
 *  進行処理
 */
void Day::Advance() {
    // 日にちのリセット
    ResetDay(); 
    // コールバック処理
    if (onAdvance) onAdvance();
}
/*
 *  ダンジョン行動
 */
void Day::ActionDungeon(Engine& engine) {
    if (morningDone) { DrawFormatString(50, 150, GetColor(255, 0, 0), "ダンジョンは午前のみ！"); return; }
    //AdvanceDay(); // 午前・午後まとめて消費
    DrawFormatString(50, 150, GetColor(0, 255, 0), "ダンジョンに行きました。午前・午後消費");
}
/*
 *  トレーニング行動
 */
void Day::ActionTraining() { AdvanceHalf("トレーニング"); }
/*
 *  ショップ行動
 */
void Day::ActionShop() { AdvanceHalf("ショップ"); }
/*
 *  アルバイト行動
 */
void Day::ActionPartTime() { AdvanceHalf("アルバイト"); }
/*
 *  半日進める処理
 */
void Day::AdvanceHalf(const char* actionName) {
    if (morningDone && afternoonDone) { DrawFormatString(50, 150, GetColor(255, 0, 0), "もう行動できません"); return; }
    if (!morningDone) { morningDone = true; DrawFormatString(50, 150, GetColor(0, 255, 0), "午前: %sしました", actionName); }
    else { afternoonDone = true; DrawFormatString(50, 150, GetColor(0, 255, 0), "午後: %sしました", actionName); }
}
/*
 *  一日進める処理
 */
void Day::AdvanceDay() { morningDone = true; afternoonDone = true; }
/*
 *  日にちのリセット処理
 */
void Day::ResetDay() { morningDone = false; afternoonDone = false; }
