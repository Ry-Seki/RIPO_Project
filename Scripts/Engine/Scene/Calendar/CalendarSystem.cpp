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
    if (day->IsFinished()) {
        GetCurrentWeek()->Advance(); // Week 内で Day を破棄
    }
}

void CalendarSystem::AdvanceDayWithFade() {
    if (fadeInProgress || FadeManager::GetInstance().IsFading())
        return; // フェード中は無視

    fadeInProgress = true;

    FadeManager::GetInstance().StartFade(
        CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop),
        [this]() { OnFadeOutComplete(); }
    );
}

void CalendarSystem::OnFadeOutComplete() {
    // 日送り実行
    AdvanceDay();

    // フェードイン開始
    FadeManager::GetInstance().StartFade(
        CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::NonStop),
        [this]() { fadeInProgress = false; } // フェード完了後にリセット
    );
}