/*
 *  @file   Day.cpp
 *  @author Seki
 */

#include "Day.h"

void Day::ActionDungeon() {
    if (morningDone) { DrawFormatString(50, 150, GetColor(255, 0, 0), "ダンジョンは午前のみ！"); return; }
    morningDone = true;
    afternoonDone = true;
    DrawFormatString(50, 150, GetColor(0, 255, 0), "ダンジョンに行きました。午前・午後消費");
}

void Day::ActionTraining() { AdvanceHalf("トレーニング"); }
void Day::ActionShop() { AdvanceHalf("ショップ"); }
void Day::ActionPartTime() { AdvanceHalf("アルバイト"); }

void Day::AdvanceHalf(const char* actionName) {
    if (morningDone && afternoonDone) { DrawFormatString(50, 150, GetColor(255, 0, 0), "もう行動できません"); return; }
    if (!morningDone) { morningDone = true; DrawFormatString(50, 150, GetColor(0, 255, 0), "午前: %sしました", actionName); }
    else { afternoonDone = true; DrawFormatString(50, 150, GetColor(0, 255, 0), "午後: %sしました", actionName); }
}

void Day::AdvanceDay() { morningDone = true; afternoonDone = true; }
void Day::ResetDay() { morningDone = false; afternoonDone = false; }
