/*
 *	@file	MenuNewGame.h
 *	@author	Seki
 */

#ifndef _MENU_NEW_GAME_H_
#define _MENU_NEW_GAME_H_

#include "../MenuBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	ニューゲームメニュー
 */
class MenuNewGame : public MenuBase {

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuNewGame() override {};

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
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close() override;

};

#endif // !_MENU_NEW_GAME_H_
