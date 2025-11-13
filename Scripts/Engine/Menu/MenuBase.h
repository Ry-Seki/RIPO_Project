/*
 *	@file	MenuBase.h
 *	@author	Seki
 */

#ifndef _MENU_BASE_H_
#define _MENU_BASE_H

/*
 *	メニューの基底クラス
 */
class MenuBase {
public:
	/*
	 *	デストラクタ
	 */
	virtual ~MenuBase() = default;
	/*
	 *	初期化処理
	 */
	virtual void Initialize() = 0;
	/*
	 *	更新処理
	 */
	virtual void Update(class Engine& engine) = 0;
	/*
	 *	描画処理
	 */
	virtual void Render() = 0;
};

#endif // !_MENU_BASE_H_
