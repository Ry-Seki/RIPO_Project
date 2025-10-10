/*
 *	@file	CalendarSystem.h
 *	@author	Seki
 */

#ifndef _CALENDAR_SYSTEM_H_
#define _CARENDAR_SYSTEM_H_
#include <memory>
#include <vector>
#include <string>
#include "Year.h"

class CalendarSystem {
private:
    std::shared_ptr<Year> currentYear;
    bool fadeInProgress = false;

public:
    CalendarSystem() = default;

    void Initialize(int numMonths = 1, int weeksPerMonth = 5, int daysPerWeek = 6);

    std::shared_ptr<Day> GetCurrentDay();
    std::shared_ptr<Week> GetCurrentWeek();
    std::shared_ptr<Month> GetCurrentMonth();
    std::shared_ptr<Year> GetYear() { return currentYear; }

    void AdvanceDay();   // åªç› Day ÇêiÇﬂÇÈ
    void AdvanceDayWithFade();

private:
    void OnFadeOutComplete();
}; 
#endif // !_CALENDER_SYSTEM_H_
