/*
 *	@file	MenuGameModeSelect.h
 *	@author	Seki
 */

#ifndef _MENU_GAME_MODE_SELECT_H_
#define _MENU_GAME_MODE_SELECT_H_

#include "../MenuBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	ゲームモードを選択するメニュー
 */
class MenuGameModeSelect : public MenuBase {

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuGameModeSelect() override {};

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;
};

#endif // !_MENU_GAME_MODE_SELECT_H_