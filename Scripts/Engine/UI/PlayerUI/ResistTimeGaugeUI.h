/*
 *	@file	ResistTimeGaugeUI.h
 *	@author Riku
 */

#ifndef _RESISTTIMEGAUGEUI_H_
#define _RESISTTIMEGAUGEUI_H_

#include "PlayerUIBase.h"

class ResistTimeGaugeUI : public PlayerUIBase {
public:
	ResistTimeGaugeUI() = default;
	~ResistTimeGaugeUI() = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize() override {}
	/*
	 *	XVˆ—
	 */
	void Update(float unscaledDeltaTime) override {}
	/*
	 *	•`‰æˆ—
	 */
	void Render() override;
};

#endif // !_RESISTTIMEGAUGEUI_H_
