/*
 *	@file	Week.h
 *	@author	Seki
 */

#ifndef _WEEK_H_
#define _WEEK_H_

#include "DateBase.h"
#include "Day.h"

/*
 *  Dayの管理クラス
 */
class Week : public DateBase {
public:
    std::vector<std::shared_ptr<Day>> days;     // Day配列
    int currentDayIndex = 0;                    // 日にちカウント

    void Advance() override {
        auto day = GetCurrentDay();
        if (!day) return;

        // Day が終わっていれば削除
        if (day->IsFinished()) {
            days.erase(days.begin() + currentDayIndex);
            // 削除したので currentDayIndex はそのまま
        }
        // Day が残っていれば何もしない（次フレームで再確認）
        if (onAdvance) onAdvance();
    }
    /*
     *  一週間の行動終了フラグ
     */
    bool IsFinished() const override { return days.empty(); }
    /*
     *  現在稼働しているDayを取得
     */
    std::shared_ptr<Day> GetCurrentDay() {
        if (currentDayIndex < days.size()) return days[currentDayIndex];
        return nullptr;
    }
}; 
#endif // !_WEEK_H_
