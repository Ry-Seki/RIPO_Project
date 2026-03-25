/*
 *	@file	AmmoCountUI.h
 *	@author	Riku
 */

#ifndef _AMMOCOUNTUI_H_
#define _AMMOCOUNTUI_H_

#include "PlayerUIBase.h"

class AmmoCountUI : public PlayerUIBase {
private:
	const float START_WIDTH_POS_RATIO;	// 横幅に対する開始位置の割合
	const float START_HEIGHT_POS_RATIO;	// 縦幅に対する開始位置の割合
	const float END_WIDTH_POS_RATIO;	// 横幅に対する終了位置の割合
	const float END_POS_Y_MOVE_RATE;	// 終了位置のY座標を動かす倍率


public:
	AmmoCountUI();
	~AmmoCountUI() = default;

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
};

#endif // !_AMMOCOUNTUI_H_

