/*
 *	@file	Month.h
 *	@author	Seki
 */
#ifndef _MONTH_H_
#define _MONTH_H_

#include "DateBase.h"
#include "Week.h"

/*
 *  Weekの管理クラス
 */
class Month : public DateBase {
public:
    std::vector<std::shared_ptr<Week>> weeks;
    size_t currentWeekIndex = 0;

    void Advance() override {
        auto week = GetCurrentWeek();
        if (!week) return;

        week->Advance(); // Week 内の Day 削除処理は Week が担当

        // Week が空なら削除
        if (week->IsFinished()) {
            weeks.erase(weeks.begin() + currentWeekIndex);
            // currentWeekIndex はそのまま、次の Week が現在のインデックスに来る
        }

        if (onAdvance) onAdvance();
    }

    bool IsFinished() const override { return weeks.empty(); }

    std::shared_ptr<Week> GetCurrentWeek() {
        if (currentWeekIndex < weeks.size()) return weeks[currentWeekIndex];
        return nullptr;
    }
};
#endif // !_MONTH_H_
