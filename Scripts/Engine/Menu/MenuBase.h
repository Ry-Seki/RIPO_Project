/*
 *	@file	MenuBase.h
 *	@author	Seki
 */

#ifndef _MENU_BASE_H_
#define _MENU_BASE_H_

#include <memory>

// 前方宣言
class Engine;

/*
 *	メニューの基底クラス
 */
class MenuBase {
	bool isClose = true;		// メニュー開閉フラグ

public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~MenuBase() = default;

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize() = 0;
	/*
	 *	@brief	準備前処理
	 */
	virtual void Setup() {}
	/*
	 *	@brief	メニューが開かれた時の処理
	 */
	virtual void Open() { isClose = false; }
	/*
	 *	更新処理
	 */
	virtual void Update(Engine& engine, float unscaledDeltaTime) = 0;
	/*
	 *	描画処理
	 */
	virtual void Render() = 0;
	/*
	 *	@brief	メニューを閉じたときの処理
	 */
	virtual void Close() { isClose = true; }

public:
	/*
	 *	@brief		メニュー開閉フラグの取得
	 *	@return		bool
	 */
	inline bool IsClose() const { return isClose; }
};
// 別名定義
using MenuBasePtr = std::shared_ptr<MenuBase>;

#endif // !_MENU_BASE_H_