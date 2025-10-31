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
    std::unique_ptr<Week> week;   // Weekクラス (常に持っている想定)
    int weekIndex = 0;            // 週間カウント

public:
    int WEEK_END_COUNT = 5;       // 1ヶ月を構成する週間の数

public:
    /*
     *  コンストラクタ (Weekクラスの生成)
     */
    Month() : week(std::make_unique<Week>()) {}

public:
    /*
     *  進行処理(Week)
     */
    void Advance() override {
        if (!week) return;
        // Weekクラスの進行処理
        week->Advance();
        // Weekの終了処理
        if (week->IsFinished()) {
            // WeekCountの更新
            weekIndex++;
            if (weekIndex < WEEK_END_COUNT) return;
            // コールバック処理
            if (onAdvance) onAdvance();
        }
    }

public:
    /*
     *  一週間の行動終了フラグ
     */
    inline bool IsFinished() const override { return weekIndex >= WEEK_END_COUNT; }
    /*
     *  Weekクラスの取得
     *  @return Week&
     */
    inline Week& GetWeek() const { return *week; }
};
#endif // !_MONTH_H_
