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
	 *	初期化処理
	 */
	void Initialize() override {}
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

#endif // !_RELOADGAUGEUI_H_

