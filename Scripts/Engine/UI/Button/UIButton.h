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
	bool isEnable = false;
	bool isSelected = false;
	bool isHovered = false;
	bool isPressed = false;

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

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize();
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
	/*
	 *	@brief		選択判定
	 *	@return		bool
	 */
	inline bool IsSelected() const { return isSelected; }
	/*
	 *	@brief		選択判定の設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsSelected(bool setFlag) {
		isSelected = setFlag;
	}
};

#endif // !_UI_BUTTON_H_