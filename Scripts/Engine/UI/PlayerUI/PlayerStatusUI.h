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
	int staminaGraphHandle = -1;
	int strengthGraphHandle = -1;

	static constexpr const char* HP_PATH = "Res/PlayerUI/hp.png";
	static constexpr const char* STAMINA_PATH = "Res/PlayerUI/stamina.png";
	static constexpr const char* STRENGTH_PATH = "Res/PlayerUI/strength.png";
	const float GRAPH_WIDTH_POS_RATIO;			// 画像用の横幅に対する位置の割合
	const float TEXT_WIDTH_POS_RATIO;			// テキスト用の横幅に対する位置の割合
	const float HP_UI_HEIGHT_POS_RATIO;			// HP関連UI用の縦幅に対する位置の割合
	const float STAMINA_UI_HEIGHT_POS_RATIO;	// スタミナ関連UI用の縦幅に対する位置の割合
	const float STRENGTH_UI_HEIGHT_POS_RATIO;	// ストレングス関連UI用の縦幅に対する位置の割合

public:
	PlayerStatusUI();
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
