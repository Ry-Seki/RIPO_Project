/*
 *	@file	MenuCredit.h
 *	@author	Seki
 */

#ifndef _MENU_CREDIT_H_
#define _MENU_CREDIT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

// 前方宣言
class Engine;

/*
 *	@brief	クレジットメニュー
 */
class MenuCredit : public MenuBase {
private:
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuCredit() override {};

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
	 *	@brief	アニメーション等の更新
	 */
	void AnimUpdate(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;

private:
	/*
	 *	@brief	ボタンの押された時の処理
	 */
	void SelectButtonExecute();

};

#endif // !_MENU_CREDIT_H_