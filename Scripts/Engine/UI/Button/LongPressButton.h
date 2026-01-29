/*
 *	@file	LongPressButton.h
 *	@author	Seki
 */

#ifndef _LONG_PRESS_BUTTON_H_
#define _LONG_PRESS_BUTTON_H_

#include "UIButtonBase.h"

/*
 *	@brief	長押しボタン
 */
class LongPressButton : public UIButtonBase {
private:
	float pressTime = 0.0f;
	float holdDuration = 1.0f;
	bool isHoldCompleted = false;

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	Rect setRect
	 */
	LongPressButton(Rect setRect) 
		: UIButtonBase(setRect){
	}
	/*
	 *	@brief		デストラクタ
	 */
	~LongPressButton() override = default;
	
public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;

public:
	/*
	 *	@brief	押した瞬間
	 */
	void OnPressDown() override;
	/*
	 *	@brief	押下中
	 */
	void OnPress(float unscaledDeltaTime) override;
	/*
	 *	@brief	離した瞬間
	 */
	void OnPressUp() override;
};

#endif // !_LONG_PRESS_BUTTON_H_