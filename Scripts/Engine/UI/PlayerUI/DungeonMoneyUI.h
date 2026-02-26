/*
 *	@file	DungeonMoneyUI.h
 *	@author	kuu
 */
#ifndef _DUNGEONMONEYUI_H_
#define _DUNGEONMONEYUI_H_

#include "PlayerUIBase.h"

class DungeonMoneyUI : public PlayerUIBase {
private:

public:
	DungeonMoneyUI() = default;
	~DungeonMoneyUI() = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize() override;
	/*
	 *	XVˆ—
	 */
	void Update(float unscaledDeltaTime) override {}
	/*
	 *	•`‰æˆ—
	 */
	void Render() override;


};

#endif // !_DUNGEONMONEYUI_H_
