/*
 *	@file	Day.h
 *	@author	Seki
 */
#ifndef _DAY_H_
#define _DAY_H_

#include "DateBase.h"

#include <DxLib.h>
/*
 *  ˆê“ú‚Ìs“®ŠÇ—ƒNƒ‰ƒX
 */
class Day : public DateBase {
public:
    bool morningDone = false;
    bool afternoonDone = false;

    void Advance() override {
        ResetDay();
        if (onAdvance) onAdvance();
    }

    bool IsFinished() const override {
        return morningDone && afternoonDone;
    }

public:
    void ActionDungeon();
    void ActionTraining();
    void ActionShop();
    void ActionPartTime();
    void AdvanceHalf(const char* actionName);
    void AdvanceDay();
    void ResetDay();
};
#endif // !_DAY_H_

