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
private:
    std::unique_ptr<Day> day;   // Dayクラス(常に持っている想定)
    int dayIndex = 0;           // 日付けカウント

private:
    int DAY_END_COUNT = 6;      // 1週間を構成する日にちの数

public:
    /*
     *  コンストラクタ (Dayクラスの作成)
     */
    Week() : day(std::make_unique<Day>()){}

public:
    /*
     *  進行処理(Day)
     */
    void Advance() override {
        if (!day) return;
        // Dayの進行処理
        day->Advance();
        // Dayクラスの終了処理
        if (day->IsFinished()) {
            // DayCountの更新
            dayIndex++;
            if (dayIndex < DAY_END_COUNT) return;
            // コールバック処理
            if (onAdvance) onAdvance();
        }
    }

public:
    /*
     *  一週間の行動終了フラグ
     */
    inline bool IsFinished() const override { return dayIndex >= DAY_END_COUNT; }
    /*
     *  Dayクラスの取得
     *  @return Day&
     */
    inline Day& GetDay() { return *day; }
}; 
#endif // !_WEEK_H_
