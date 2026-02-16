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
protected:
	bool isInteractive = false;		// 操作可能フラグ
	bool isVisible = false;			// 描画フラグ
	bool isStart = false;			// 開始フラグ
	bool inputHandle = false;		// 入力フラグ
	bool isAnimation = false;		// アニメーションフラグ

public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~MenuBase() {}

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize(Engine& engine) = 0;
	/*
	 *	@brief	準備前処理
	 */
	virtual void Setup() {}
	/*
	 *	@brief	メニューが開かれた時の処理
	 */
	virtual void Open() {
		isVisible = true;
		isInteractive = true;
		isStart = false;
		inputHandle = false;
		isAnimation = true;
	}
	/*
	 *	@brief	更新処理
	 */
	virtual void Update(Engine& engine, float unscaledDeltaTime){}
	/*
	 *	@brief	アニメーション等の更新
	 */
	virtual void AnimUpdate(Engine& engine, float unscaledDeltaTime){}
	/*
	 *	@brief	描画処理
	 */
	virtual void Render() = 0;
	/*
	 *	@brief	メニューを閉じたときの処理
	 */
	virtual void Close(Engine& engine) {
		isVisible = false;
		isInteractive = false;
		isStart = false;
		isAnimation = false;
	}
	/*
	 *	@brief	メニューが中断されたときの処理
	 */
	virtual void Suspend() {
		isInteractive = false;
	}
	/*
	 *	@brief	メニューが再開されたときの処理
	 */
	virtual void Resume() {
		isVisible = true;
		isInteractive = true;
		isStart = true;
		inputHandle = false;
	}

public:
	/*
	 *	@brief		描画判定
	 *	@return		bool
	 */
	inline bool IsVisible() const { return isVisible; }
	/*
	 *	@brief		描画フラグの変更
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsVisible(bool setFlag) { isVisible = setFlag; }
	/*
	 *	@brief		操作可能判定
	 *	@return		bool
	 */
	inline bool IsInteractive() const { return isInteractive; }
	/*
	 *	@brief		操作可能フラグの変更
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsInteractive(bool setFlag) { isInteractive = setFlag; }
	/*
	 *	@brief		アニメーション判定
	 *	@return		bool
	 */
	inline bool IsAnimation() const { return isAnimation; }
	/*
	 *	@brief		アニメーション判定の変更
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsAnimation(bool setFlag) { isAnimation = setFlag; }
};

#endif // !_MENU_BASE_H_