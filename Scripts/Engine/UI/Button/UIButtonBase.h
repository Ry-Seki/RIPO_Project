/*
 *	@file	UIButtonBase.h
 *	@author	Seki
 */

#ifndef _UI_BUTTON_BASE_H_
#define _UI_BUTTON_BASE_H_

#include "IButtonStateRenderer.h"
#include "../Rect.h"
#include "../../GameEnum.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

/*
 *	@brief	UIボタンの基底クラス
 */
class UIButtonBase {
protected:		// 別名定義
	using IButtonStateRendererPtr = std::shared_ptr<IButtonStateRenderer>;

protected:
	int inputClickNum = -1;
	bool isEnable = false;
	bool isVisible = false;
	bool inputHandle = false;
	GameEnum::ButtonInputState inputState = GameEnum::ButtonInputState::Invalid;
	GameEnum::ButtonSelectState selectState = GameEnum::ButtonSelectState::Invalid;

	Rect rect;
	std::string name = "";
	std::vector<int> handleList;							
	std::vector<IButtonStateRendererPtr> rendererList;	// TODO : そのうち追加

	std::function<void()> OnClick = nullptr;
	std::function<void()> UpdateSelectButton = nullptr;

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	Rect setRect
	 */
	UIButtonBase(Rect setRect)
		: rect(setRect){
	}
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~UIButtonBase() {}

protected:
	/*
	 *	@brief	発火イベント
	 */
	virtual void OnClickEvent();
	/*
	 *	@brief	デバック用描画
	 */
	virtual void DebugRender();
	/*
	 *	@brief		描画状態の取得
	 *	@return		GameEnum::ButtonRendererState
	 */
	GameEnum::ButtonRendererState GetRendererState() const;
	/*
	 *	@brief		押していたものが離れた瞬間を判定
	 *	@return		bool
	 */
	bool OnReleasedUp() const;

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize();
	/*
	 *	@brief	準備前処理
	 */
	virtual void Setup();
	/*
	 *	@brief	更新処理
	 */
	virtual void Update(float unscaledDeltaTime);
	/*
	 *	@brief	描画処理
	 */
	virtual void Render() = 0;
		
public:
	/*
	 *	@brief	押した瞬間
	 */
	virtual void OnPressDown() = 0;
	/*
	 *	@brief	押下中
	 */
	virtual void OnPress(float unscaledDeltaTime) = 0;
	/*
	 *	@brief	離した瞬間
	 */
	virtual void OnPressUp() = 0;
	/*
	 *	@brief	キャンセル
	 */
	virtual void OnPressCancel(){}

public:
	/*
	 *	@brief		操作対象か判定
	 *	@return		bool
	 */
	inline bool IsFocus() const {
		return inputState == GameEnum::ButtonInputState::Hover
			|| selectState == GameEnum::ButtonSelectState::Select;
	}
	/*
	 *	@brief		操作可能判定
	 *	@return		bool
	 */
	inline bool IsEnable() const { return isEnable; }
	/*
	 *	@brief		操作可能フラグの設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsEnable(bool setFlag) {
		isEnable = setFlag;
	}
	/*
	 *	@brief		描画判定
	 *	@return		bool
	 */
	inline bool IsVisible() const { return isVisible; }
	/*
	 *	@brief		描画判定の設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsVisible(bool setFlag) { isVisible = setFlag; }

	/*
	 *	@brief		ボタン状態の取得
	 *	@return		GameEnum::ButtonInputState
	 */
	inline GameEnum::ButtonInputState GetInputState() const {
		return inputState;
	}
	/*
	 *	@brief		名前の取得
	 *	@return		std::string
	 */
	inline std::string GetName() const { return name; }
	/*
	 *	@brief		名前の設定
	 *	@param[in]	const std::string& setName
	 */
	inline void SetName(const std::string& setName) {
		name = setName;
	}
	/*
	 *	@brief		画像ハンドルの登録
	 *	@param[in]	int index
	 *	@param[in]	int setHandle
	 */
	inline void RegisterButtonHandle(int index, int setHandle) {
		if (setHandle == -1 || index > handleList.size()) return;

		handleList[index] = setHandle;
	}
	/*
	 *	@brief		発火イベントの登録
	 *	@param[in]	std::function<void()> setFunction
	 */
	inline void RegisterOnClick(std::function<void()> setFunction) {
		if (setFunction) OnClick = setFunction;
	}
	/*
	 *	@brief		選択状態更新処理の設定
	 *  @param[in]	std::function<void()> setFunction
	 */
	inline void RegisterUpdateSelectButton(std::function<void()> setFunction) {
		if (setFunction) UpdateSelectButton = setFunction;
	}
	/*
	 *	@brief		選択状態か判定
	 *	@return		bool
	 */
	inline bool IsSelect() const {
		return selectState == GameEnum::ButtonSelectState::Select;
	}
	/*
	 *	@brief		選択状態の設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetSelectState(bool setFlag) {
		if (setFlag) {
			selectState = GameEnum::ButtonSelectState::Select;
		} else {
			selectState = GameEnum::ButtonSelectState::Invalid;
		}
	}
};

#endif // !_UI_BUTTON_BASE_H_