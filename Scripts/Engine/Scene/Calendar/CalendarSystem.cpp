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
/*
 *  現在稼働しているDay取得
 */
std::shared_ptr<Day> CalendarSystem::GetCurrentDay() {
    auto month = currentYear->GetCurrentMonth();
    if (!month) return nullptr;
    auto week = month->GetCurrentWeek();
    if (!week) return nullptr;
    return week->GetCurrentDay();
}
/*
 *  現在稼働しているWeek取得
 */
std::shared_ptr<Week> CalendarSystem::GetCurrentWeek() {
    auto month = currentYear->GetCurrentMonth();
    if (!month) return nullptr;
    return month->GetCurrentWeek();
}
/*
 *  現在稼働しているMonth取得
 */
std::shared_ptr<Month> CalendarSystem::GetCurrentMonth() {
    if (!currentYear) return nullptr;
    return currentYear->GetCurrentMonth();
}
/*
 *  現在稼働しているYear取得
 */
std::shared_ptr<Year> CalendarSystem::GetYear() {
    return currentYear;
}
/*
 *  日にちの進行
 */
void CalendarSystem::AdvanceDay() {
    auto day = GetCurrentDay();
    if (!day) return;

    day->AdvanceDay();  // 1日を完全に消費

    auto week = GetCurrentWeek();
    if (week && day->IsFinished()) week->Advance();

    auto month = GetCurrentMonth();
    if (month && week && week->IsFinished()) month->Advance();

    if (currentYear) currentYear->Advance();  // Year も同様に
}