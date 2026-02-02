/*
 *	@file	MenuInGame.h
 *	@author	Seki
 */

#ifndef _MENU_IN_GAME_H_
#define _MENU_IN_GAME_H_

#include "../MenuBase.h"

/*
 *	@brief	インゲームメニュー
 */
class MenuInGame : public MenuBase {
private:

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
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
	void Close() override;
	/*
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;
};

#endif // !_MENU_IN_GAME_H_