/*
 *	@file	MenuSelectLoadGame.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_LOAD_GAME_H_
#define _MENU_SELECT_LOAD_GAME_H_

#include "../MenuBase.h"
#include "../../GameEnum.h"

// 前方宣言
class Engine;

/*
 *	@brief	ロードメニュー
 */
class MenuSelectLoadGame : public MenuBase {
private:
	GameEnum::SaveSlotMenuMode mode = GameEnum::SaveSlotMenuMode::Load;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectLoadGame() override {};

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

#endif // !_MENU_SELECT_LOAD_GAME_H_