/*
 *	@file	ReloadGaugeUI.h
 *	@author	Riku
 */

#ifndef _RELOADGAUGEUI_H_
#define _RELOADGAUGEUI_H_

#include "PlayerUIBase.h"

class ReloadGaugeUI : public PlayerUIBase {
public:
	ReloadGaugeUI() = default;
	~ReloadGaugeUI() = default;

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

#endif // !_RELOADGAUGEUI_H_

