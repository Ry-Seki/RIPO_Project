/*
 *	@file	DungeonMoneyUI.h
 *	@author	kuu
 */
#ifndef _DUNGEONMONEYUI_H_
#define _DUNGEONMONEYUI_H_

#include "PlayerUIBase.h"
#include "../../Manager/FontManager.h"

class DungeonMoneyUI : public PlayerUIBase {
private:
	// 表示時間
	float displayTime = 0;
	// 1F前の所持金(常に更新し続ける)
	int prevMoney = 0;
	// 所持金画像のハンドル
	int MoneyGraphHandle = -1;

	static constexpr const char* MONEY_PATH = "Res/PlayerUI/money.png";

public:
	DungeonMoneyUI() = default;
	~DungeonMoneyUI() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
	/*
	 *	更新処理
	 */
	void Update(float unscaledDeltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;

private :
	/*
	 *	所持金加算表示
	 */
	void AddMoneyUI(FontManager& font);

};

#endif // !_DUNGEONMONEYUI_H_
