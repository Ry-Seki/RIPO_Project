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
    std::vector<std::shared_ptr<Month>> months;     // Month配列
    int currentMonthIndex = 0;                      // 月カウント

public:
    /*
     *  進行処理
     */
    void Advance() override {
        auto month = GetCurrentMonth();
        if (!month) return;

        // 進行処理の呼び出し
        month->Advance();

        if (month->IsFinished()) {
            months.erase(months.begin() + currentMonthIndex);
            // currentMonthIndex はそのまま、次の Month が現在のインデックスに来る
        }
        // 進行処理のコールバック
        if (onAdvance) onAdvance();
    }
    /*
     *  一か月行動終了フラグ
     */
    bool IsFinished() const override { return months.empty(); }
    /*
     *  現在稼働しているMonthの取得
     */
    std::shared_ptr<Month> GetCurrentMonth() {
        if (currentMonthIndex < months.size()) return months[currentMonthIndex];
        return nullptr;
    }
};
#endif // !_YEAR_H_
