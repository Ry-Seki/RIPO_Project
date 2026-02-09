/*
 *	@file	AmmoCountUI.h
 *	@author	Riku
 */

#ifndef _AMMOCOUNTUI_H_
#define _AMMOCOUNTUI_H_

#include "PlayerUIBase.h"

class AmmoCountUI : public PlayerUIBase {
public:
	AmmoCountUI() = default;
	~AmmoCountUI() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override{}
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

#endif // !_AMMOCOUNTUI_H_

