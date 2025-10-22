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
    std::unique_ptr<Month> month;   // Monthクラス (常に持っている想定)
    int monthIndex = 0;      // 月カウント

public:
    int MONTH_END_COUNT = 12;       // 1年を更新する月の数

public:
    /*
     *  コンストラクタ (Monthクラスの生成)
     */
    Year() : month(std::make_unique<Month>()) {}

public:
    /*
     *  進行処理 (Month)
     */
    void Advance() override {
        if (!month) return;
        // Monthクラスの進行処理
        month->Advance();
        if (month->IsFinished()) {
            // MonthCountの更新
            monthIndex++;
            if (monthIndex < MONTH_END_COUNT) return;
            // コールバック処理
            if (onAdvance) onAdvance();
        }
    }
    /*
     *  一か月行動終了フラグ
     */
    inline bool IsFinished() const override { return monthIndex >= MONTH_END_COUNT; }
    /*
     *  Monthクラスの取得
     *  @return Month&
     */
    inline Month& GetMonth() const { return *month; }
};
#endif // !_YEAR_H_
