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

class CalendarManager {
private:
    std::shared_ptr<CalendarSystem> calendarSystem;

    bool inputHandle = false; // 1フレーム1回の入力制御
    bool isFading = false;    // フェード中フラグ

public:
    CalendarManager(std::shared_ptr<CalendarSystem> system) : calendarSystem(system) {}

    void Update();   // 入力処理と日進行
    void Render();   // 描画

private:
    void StartDayAdvanceWithFade(); // フェード付き日進行
}; 
#endif // !_CARENDER_MANAGER_H_

