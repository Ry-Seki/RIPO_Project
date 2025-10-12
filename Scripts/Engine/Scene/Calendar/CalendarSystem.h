/*
 *	@file	CalendarSystem.h
 *	@author	Seki
 */

#ifndef _CALENDAR_SYSTEM_H_
#define _CARENDAR_SYSTEM_H_

#include <memory>
#include <vector>
#include <string>
#include "Year.h"

/*
 *  カレンダー内部処理クラス
 */
class CalendarSystem {
private:
    std::shared_ptr<Year> currentYear;  // 年
    bool fadeInProgress = false;        // フェード進行フラグ

public:
    /*
     *  コンストラクタ
     */
    CalendarSystem() = default;
    /*
     *  初期化処理
     *  param[in]       int numMonths       生成する月の数
     *  param[in]       int weeksPerMonth   生成する週間の数
     *  param[in]       int daysPerWeek     生成する日にちの数
     */
    void Initialize(int numMonths = 1, int weeksPerMonth = 5, int daysPerWeek = 6);
    /*
     *  現在稼働しているDay取得
     */
    std::shared_ptr<Day> GetCurrentDay();
    /*
     *  現在稼働しているWeek取得
     */
    std::shared_ptr<Week> GetCurrentWeek();
    /*
     *  現在稼働しているMonth取得
     */
    std::shared_ptr<Month> GetCurrentMonth();
    /*
     *  現在稼働しているYear取得
     */
    std::shared_ptr<Year> GetYear();

    void AdvanceDay();   // 現在 Day を進める
}; 
#endif // !_CALENDER_SYSTEM_H_
