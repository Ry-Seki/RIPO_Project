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
    std::vector<std::shared_ptr<Week>> weeks;   // Week配列
    int currentWeekIndex = 0;                   // 週間カウント

    /*
     *  進行処理
     */
    void Advance() override {
        auto week = GetCurrentWeek();
        if (!week) return;

        week->Advance(); // Week 内の Day 削除処理は Week が担当

        // Week が空なら削除
        if (week->IsFinished()) {
            weeks.erase(weeks.begin() + currentWeekIndex);
            // currentWeekIndex はそのまま、次の Week が現在のインデックスに来る
        }
        // 進行処理コールバック
        if (onAdvance) onAdvance();
    }
    /*
     *  一週間の行動終了フラグ
     */
    bool IsFinished() const override { return weeks.empty(); }
    /*
     *  現在稼働しているWeekの取得
     */
    std::shared_ptr<Week> GetCurrentWeek() {
        if (currentWeekIndex < weeks.size()) return weeks[currentWeekIndex];
        return nullptr;
    }
};
#endif // !_MONTH_H_
