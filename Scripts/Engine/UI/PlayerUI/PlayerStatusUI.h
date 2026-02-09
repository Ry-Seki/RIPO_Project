/*
 *	@file	PlayerStatusUI.h
 *	@author Riku
 */

#ifndef _PLAYERSTATUSUI_H_
#define _PLAYERSTATUSUI_H_

#include "PlayerUIBase.h"

class PlayerStatusUI : public PlayerUIBase{
private:
	int HPGraphHandle = -1;
	int StaminaGraphHandle = -1;
	int StrengthGraphHandle = -1;

public:
	PlayerStatusUI() = default;
	~PlayerStatusUI() = default;

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
	void SetGraphHandle(int setHPHandle, int setStaminaHandle, int setStrengthHandle);
};

#endif // !_PLAYERSTATUSUI_H_
