/*
 *	@file	SinglePressButton.h
 *	@author	Seki
 */

#ifndef __SINGLE_PRESS_BUTTON_H_
#define __SINGLE_PRESS_BUTTON_H_

#include "UIButtonBase.h"
#include "IButtonStateRenderer.h"
#include "../Rect.h"
#include "../../GameEnum.h"

#include <string>
#include <memory>
#include <functional>

/*
 *	@brief	単押しボタンクラス(UnityのようなinputUp時にイベント発火)
 */
class SinglePressButton : public UIButtonBase {
private:

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	Rect setRect
	 */
	SinglePressButton(Rect setRect)
		: UIButtonBase(setRect) {
	}
	/*
	 *	@brief		デストラクタ
	 */
	~SinglePressButton() override = default;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
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

#endif // !__SINGLE_PRESS_BUTTON_H_