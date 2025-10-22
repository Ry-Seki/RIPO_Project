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
    std::unique_ptr<Year> year;     // 年 (常に持っている想定)
    int yearIndex = 0;              // 年カウント
    bool fadeInProgress = false;    // フェード進行フラグ
    
public:
    /*
     *  コンストラクタ
     */
    CalendarSystem() = default;
    /*
     *  デストラクタ
     */
    ~CalendarSystem() = default;

public:
    /*
     *  初期化処理
     */
    void Initialize();
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    void Setup();
    /*
     *  Dayを進める
     */
    void AdvanceDay();

public:
    /*
     *  Dayクラスの取得
     *  @return Day&
     */
    inline Day& GetDay() { return GetWeek().GetDay(); }
    /*
     *  Weekクラスの取得
     *  @return Week&
     */
    inline Week& GetWeek() { return GetMonth().GetWeek(); }
    /*
     *  Monthクラスの取得
     *  @return Month&
     */
    inline Month& GetMonth() { return GetYear().GetMonth(); }
    /*
     *  Yearクラスの取得
     *  @return Year&
     */
    inline Year& GetYear() { return *year; }
}; 
#endif // !_CALENDER_SYSTEM_H_
