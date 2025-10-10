/*
 *	@file	Year.h
 *	@author	Seki
 */
#ifndef _YEAR_H_
#define _YEAR_H_

#include "DateBase.h"
#include "Month.h"

/*
 *  Month‚ğŠÇ—‚·‚éƒNƒ‰ƒX
 */
class Year : public DateBase {
public:
    std::vector<std::shared_ptr<Month>> months;
    size_t currentMonthIndex = 0;

    void Advance() override {
        if (currentMonthIndex < months.size()) {
            months[currentMonthIndex]->Advance();
            if (months[currentMonthIndex]->IsFinished()) months.erase(months.begin() + currentMonthIndex);
            else ++currentMonthIndex;
            if (onAdvance) onAdvance();
        }
    }

    bool IsFinished() const override { return months.empty(); }

    std::shared_ptr<Month> GetCurrentMonth() {
        if (currentMonthIndex < months.size()) return months[currentMonthIndex];
        return nullptr;
    }
};
#endif // !_YEAR_H_
