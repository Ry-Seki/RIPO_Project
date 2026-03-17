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
	const float GAUGE_START_WIDTH_POS_RATIO;	// ゲージ用の横幅に対する開始位置の割合
	const float GAUGE_START_HEIGHT_POS_RATIO;	// ゲージ用の縦幅に対する開始位置の割合
	const float GAUGE_END_POS_X_MOVE_RATE;		// ゲージ用の終了位置のX座標を動かす倍率
	const float GAUGE_END_HEIGHT_POS_RATIO;		// ゲージ用の縦幅に対する終了位置の割合
	const float GRAPH_WIDTH_POS_RATIO;			// 画像用の横幅に対する位置の割合
	const float GRAPH_HEIGHT_POS_RATIO;			// 画像用の縦幅に対する位置の割合

public:
	ResistTimeGaugeUI();
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
