/*
 *	@file	Month.h
 *	@author	Seki
 */
#ifndef _MONTH_H_
#define _MONTH_H_

#include "DateBase.h"
#include "Week.h"

/*
 *  WeekÇÃä«óùÉNÉâÉX
 */
class Month : public DateBase {
public:
    std::vector<std::shared_ptr<Week>> weeks;
    size_t currentWeekIndex = 0;

    void Advance() override {
        if (currentWeekIndex < weeks.size()) {
            weeks[currentWeekIndex]->Advance();
            // èIóπÇµÇΩWeekÇë¶çÌèú
            if (weeks[currentWeekIndex]->IsFinished()) weeks.erase(weeks.begin() + currentWeekIndex);
            else ++currentWeekIndex;
            if (onAdvance) onAdvance();
        }
    }

    bool IsFinished() const override { return weeks.empty(); }

    std::shared_ptr<Week> GetCurrentWeek() {
        if (currentWeekIndex < weeks.size()) return weeks[currentWeekIndex];
        return nullptr;
    }
};
#endif // !_MONTH_H_
