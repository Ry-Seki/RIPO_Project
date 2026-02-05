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
	float holdElapsed = 0.0f;		  // 長押し経過時間
	float repeatElapsed = 0.0f;		  // 繰り返し経過時間

	float initialDelay = 0.5f;        // 即時後の待機
	float repeatInterval = 0.3f;      // 初期間隔
	float minRepeatInterval = 0.05f;  // 下限
	float accelerationRate = 0.15f;   // 秒あたり短縮量

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