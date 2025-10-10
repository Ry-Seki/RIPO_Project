/*
 *	@file	Week.h
 *	@author	Seki
 */
#ifndef _WEEK_H_
#define _WEEK_H_

#include "DateBase.h"
#include "Day.h"

/*
 *  Day‚ÌŠÇ—ƒNƒ‰ƒX
 */
class Week : public DateBase {
public:
    std::vector<std::shared_ptr<Day>> days;
    size_t currentDayIndex = 0;

    void Advance() override {
        if (currentDayIndex < days.size()) {
            days[currentDayIndex]->Advance();
            // I—¹‚µ‚½Day‚ğ‘¦íœ
            if (days[currentDayIndex]->IsFinished()) days.erase(days.begin() + currentDayIndex);
            else ++currentDayIndex;
            if (onAdvance) onAdvance();
        }
    }

    bool IsFinished() const override { return days.empty(); }

    std::shared_ptr<Day> GetCurrentDay() {
        if (currentDayIndex < days.size()) return days[currentDayIndex];
        return nullptr;
    }
}; 
#endif // !_WEEK_H_
