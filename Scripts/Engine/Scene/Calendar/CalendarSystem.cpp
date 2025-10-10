/*
 *  @file   CalendarSystem.cpp
 *  @author Seki
 */

#include "CalendarSystem.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"

void CalendarSystem::Initialize(int numMonths, int weeksPerMonth, int daysPerWeek) {
    currentYear = std::make_shared<Year>();
    for (int m = 0; m < numMonths; ++m) {
        auto month = std::make_shared<Month>();
        for (int w = 0; w < weeksPerMonth; ++w) {
            auto week = std::make_shared<Week>();
            for (int d = 0; d < daysPerWeek; ++d) {
                week->days.push_back(std::make_shared<Day>());
            }
            month->weeks.push_back(week);
        }
        currentYear->months.push_back(month);
    }
}

std::shared_ptr<Day> CalendarSystem::GetCurrentDay() {
    auto month = currentYear->GetCurrentMonth();
    if (!month) return nullptr;
    auto week = month->GetCurrentWeek();
    if (!week) return nullptr;
    return week->GetCurrentDay();
}

std::shared_ptr<Week> CalendarSystem::GetCurrentWeek() {
    auto month = currentYear->GetCurrentMonth();
    if (!month) return nullptr;
    return month->GetCurrentWeek();
}

std::shared_ptr<Month> CalendarSystem::GetCurrentMonth() {
    if (!currentYear) return nullptr;
    return currentYear->GetCurrentMonth();
}

void CalendarSystem::AdvanceDay() {
    auto day = GetCurrentDay();
    if (!day) return;

    day->AdvanceDay();  // 1“ú‚ðŠ®‘S‚ÉÁ”ï

    auto week = GetCurrentWeek();
    if (week && day->IsFinished()) week->Advance();

    auto month = GetCurrentMonth();
    if (month && week && week->IsFinished()) month->Advance();

    if (currentYear) currentYear->Advance();  // Year ‚à“¯—l‚É
}