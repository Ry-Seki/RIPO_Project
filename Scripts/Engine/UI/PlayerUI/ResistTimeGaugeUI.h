/*
 *	@file	ResistTimeGaugeUI.h
 *	@author Riku
 */

#ifndef _RESISTTIMEGAUGEUI_H_
#define _RESISTTIMEGAUGEUI_H_

#include "PlayerUIBase.h"

class ResistTimeGaugeUI : public PlayerUIBase {
private:
	int resistTimeGraphHandle = -1;

	static constexpr const char* RESIST_PATH = "Res/PlayerUI/resist.png";

public:
	ResistTimeGaugeUI() = default;
	~ResistTimeGaugeUI() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
	/*
	 *	更新処理
	 */
	void Update(float unscaledDeltaTime) override {}
	/*
	 *	描画処理
	 */
	void Render() override;

public:
	/*
	 *	グラフハンドルのセット
	 */
	void SetGraphHandle(int setHandle);
};

#endif // !_RESISTTIMEGAUGEUI_H_
