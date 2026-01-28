/*
 *	@file	UIButton.h
 *	@author	Seki
 */

#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "IButtonStateRenderer.h"
#include "../Rect.h"
#include "../../GameEnum.h"

#include <string>
#include <memory>
#include <functional>

/*
 *	@brief	UIボタンクラス
 */
class UIButton {
public:		// 別名定義
	using IButtonStateRendererPtr = std::shared_ptr<IButtonStateRenderer>;
private:
	GameEnum::ButtonState buttonState = GameEnum::ButtonState::Invalid;

	Rect rect;
	std::string name = "";


	std::vector<int> buttonHandleList;
	std::vector<IButtonStateRendererPtr> rendererList;
	IButtonStateRendererPtr currentRenderer = nullptr;

	std::function<void()> onClick = nullptr;

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	Rect setRect
	 */
	UIButton(Rect setRect)
		: rect(setRect) {
	}
	/*
	 *	@brief		デストラクタ
	 */
	~UIButton() = default;

private:
	/*
	 *	@brief	デバック用描画
	 */
	void DebugRender();

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief	準備前処理
	 */
	void Setup();
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime);
	/*
	 *	@brief	実行処理
	 */
	void Execute();
	/*
	 *	@brief	描画処理
	 */
	void Render();

public:
	/*
	 *	@brief		ボタン状態の取得
	 *	@return		GameEnum::ButtonState
	 */
	GameEnum::ButtonState GetButtonState() const;

public:
	/*
	 *	@brief		名前の設定
	 *	@param[in]	const std::string& setName
	 */
	inline void SetName(const std::string& setName) {
		name = setName;
	}
	/*
	 *	@brief		画像ハンドルの登録
	 *	@param[in]	int setHandle
	 */
	inline void RegisterButtonHandle(int setHandle) {
		if (setHandle == -1) return;

		buttonHandleList.push_back(setHandle);
	}
	/*
	 *	@brief		押されたときの処理を登録
	 *	@param[in]	std::function<void()> setOnClick
	 */
	inline void SetOnClick(std::function<void()> setOnClick) {
		onClick = setOnClick;
	}
};

#endif // !_UI_BUTTON_H_