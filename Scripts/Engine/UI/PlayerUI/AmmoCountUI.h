/*
 *	@file	AmmoCountUI.h
 *	@author	Riku
 */

#ifndef _AMMOCOUNTUI_H_
#define _AMMOCOUNTUI_H_

#include "PlayerUIBase.h"

class AmmoCountUI : public PlayerUIBase {
private:
	const float WIDTH_POS_RATIO;	// 横幅に対する位置の割合
	const float HEIGHT_POS_RATIO;	// 縦幅に対する位置の割合

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

