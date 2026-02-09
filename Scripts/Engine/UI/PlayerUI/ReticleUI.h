/*
 *	@file	ReticleUI.h
 *	@author	Riku
 */

#ifndef _RETICLEUI_H_
#define _RETICLEUI_H_

#include "PlayerUIBase.h"

class ReticleUI : public PlayerUIBase{
private:
	int reticleGraphHandle = -1;

	static constexpr const char* RETICLE_PATH = "Res/PlayerUI/reticle.png";

public:
	ReticleUI() = default;
	~ReticleUI() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
	/*
	 *	更新処理
	 */
	void Update(float unscaledDeltaTime) override{}
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

#endif // !_RETICLEUI_H_

