/*
 *	@file	Year.h
 *	@author	Seki
 */
#ifndef _YEAR_H_
#define _YEAR_H_

#include "DateBase.h"
#include "Month.h"

/*
 *  Monthを管理するクラス
 */
class Year : public DateBase {
public:
    std::vector<std::shared_ptr<Month>> months;
    size_t currentMonthIndex = 0;

    void Advance() override {
        auto month = GetCurrentMonth();
        if (!month) return;

        month->Advance(); // Month 内の Week 削除処理は Month が担当

        if (month->IsFinished()) {
            months.erase(months.begin() + currentMonthIndex);
            // currentMonthIndex はそのまま、次の Month が現在のインデックスに来る
        }

        if (onAdvance) onAdvance();
    }

    bool IsFinished() const override { return months.empty(); }

    std::shared_ptr<Month> GetCurrentMonth() {
        if (currentMonthIndex < months.size()) return months[currentMonthIndex];
        return nullptr;
    }
};
#endif // !_YEAR_H_
