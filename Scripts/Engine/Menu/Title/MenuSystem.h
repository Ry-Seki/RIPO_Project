/*
 *	@file	MenuSystem.h
 *	@author	Seki
 */

#ifndef _MENU_SYSTEM_H_
#define _MENU_SYSTEM_H_

#include "../MenuBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	システムメニュー
 */
class MenuSystem : public MenuBase {

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSystem() override {};

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

};

#endif // !_MENU_SYSTEM_H_
