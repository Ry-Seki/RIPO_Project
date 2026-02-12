/*
 *	@file	MenuSelectAction.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_ACTION_H_
#define _MENU_SELECT_ACTION_H_

#include "../MenuBase.h"

/*
 *	@brief	アクション選択メニュー
 */
class MenuSelectAction : public MenuBase {
private:

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectAction() override {}

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
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);
};

#endif // !_MENU_SELECT_ACTION_H_