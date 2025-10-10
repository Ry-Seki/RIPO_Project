/*
 *	@file	Day.h
 *	@author	Seki
 */
#ifndef _DAY_H_
#define _DAY_H_

#include "DateBase.h"

#include <DxLib.h>
/*
 *  一日の行動管理クラス
 */
class Day : public DateBase {
public:
    bool morningDone = false;
    bool afternoonDone = false;

    void Advance() override { ResetDay(); if (onAdvance) onAdvance(); }
    bool IsFinished() const override { return morningDone && afternoonDone; }

public:
    void ActionDungeon();     // 午前・午後消費
    void ActionTraining();    // 半日消費
    void ActionShop();        // 半日消費
    void ActionPartTime();    // 半日消費
    void AdvanceHalf(const char* actionName);
    void AdvanceDay();        // 全日消費
    void ResetDay();
}; 
#endif // !_DAY_H_

