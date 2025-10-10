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
    bool inputHandle = false;

public:
    CalendarManager(const std::shared_ptr<CalendarSystem>& system) : calendarSystem(system) {}
    void Update(); // “ü—Íˆ—
    void Render(); // •`‰æˆ—

    void NextDay();       // “ú•t‚ği‚ß‚é‚¾‚¯
    bool IsDayComplete() const;
};

#endif // !_CARENDER_MANAGER_H_

