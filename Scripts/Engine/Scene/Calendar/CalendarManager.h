/*
 *	@file	CalendarManager.h
 *	@author	Seki
 */

#ifndef _CARENDAR_MANAGER_H_
#define _CARENDAR_MANAGER_H_

#include "CalendarSystem.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"

#include <DxLib.h>

// 前方宣言
class Engine;
 
/*
 *  カレンダー管理クラス
 */
class CalendarManager {
private:
    std::shared_ptr<CalendarSystem> calendarSystem;     // カレンダーシステム
    bool inputHandle = false;                           // 入力フラグ
    bool isActive = true;                               // 行動フラグ

public:
    /*
     *  コンストラクタ
     *  param[in]           const std::shared_ptr<CalendarSystem>& system   カレンダー内部処理クラスのポインタ
     */
    CalendarManager(const std::shared_ptr<CalendarSystem>& system) : calendarSystem(system) {}
    /*
     *  デストラクタ
     */
    ~CalendarManager() = default;

public:
    /*
     *  更新処理
     */
    void Update(Engine& engine);
    /*
     *  描画処理
     */
    void Render(); 
    /*
     *  次の日に進行処理
     */
    void NextDay();

public:
    /*
     *  一日の行動終了フラグ取得
     */
    bool IsDayComplete() const;
};

#endif // !_CARENDER_MANAGER_H_

