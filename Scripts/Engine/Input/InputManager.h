/*
 *	@file	InputManager.h
 *  @author	Seki
 */

#pragma once
#include "../Singleton.h"

#include "ActionMapBase.h"
#include "../GameEnum.h"

#include <DxLib.h>
#include <unordered_map>
#include <memory>

/*
 *	入力管理クラス
 */
class InputManager : public Singleton<InputManager>{
	// フレンド宣言
	friend class Singleton<InputManager>;
private:	//メンバ変数
	char keyState[256];
	char prevKeyState[256];
	int nowMouseInput;
	int prevMouseInput;
	
	std::unordered_map<GameEnum::ActionMap, std::shared_ptr<ActionMapBase>> actionMaps;		// 各アクションマップ
private:	    //コンストラクタとデストラクタ
	/*
	 *	コンストラクタ
	 */
	InputManager();
	/*
	 *	デストラクタ
	 */
	~InputManager();

public:		//メンバ関数
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	更新処理
	 */
	void Update();

public:		//キーボード用入力管理
	/*
	 *	キーが押されたかどうか
	 *	@param[in]	int _key			　	キー番号
	 *	@return		bool
	 */
	inline bool IsKeyDown(int _key) const {
		// !前 && 今
		return !prevKeyState[_key] && keyState[_key];
	}
	/*
	 *	キーを押しているかどうか
	 *	@param[in]	int _key				キー番号
	 *	@return		bool
	 */
	inline bool IsKey(int _key) const {
		// 今
		return keyState[_key];
	}
	/*
	 *	キーが押されたかどうか
	 *	@param[in]	int _key				キー番号
	 *	@return		bool
	 */
	inline bool IsKeyUp(int _key) const {
		// 前 && !今
		return prevKeyState[_key] && !keyState[_key];
	}
public:		//マウス用入力管理
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	int _mouseButton		マウスボタン番号
	 *	@return		bool
	 */
	inline bool IsMouseDown(int _mouseButton) const {
		return !static_cast<bool>(prevMouseInput & _mouseButton) && 
			static_cast<bool>(nowMouseInput & _mouseButton);
	}
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	int _mouseButton		マウスボタン番号
	 *	@return		bool
	 */
	inline bool IsMouse(int _mouseButton) const {
		return static_cast<bool>(nowMouseInput & _mouseButton);
	}
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	int _mouseButton		マウスボタン番号
	 *	@return		bool
	 */
	inline bool IsMouseUp(int _mouseButton) const {
		return static_cast<bool>(prevMouseInput & _mouseButton) && 
			!static_cast<bool>(nowMouseInput & _mouseButton);
	}
public: // マウスとキーの両方入力
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	ActionMapBase::Input _mouseButton	入力
	 *	@return		bool
	 *  @outhor Riku
	 */
	inline bool IsInputDown(ActionMapBase::Input _button) const {
		switch (_button.type) {
		case ActionMapBase::InputType::Key:
			return IsKeyDown(_button.input);
		case ActionMapBase::InputType::Mouse:
			return IsMouseDown(_button.input);
		}
	}
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	ActionMapBase::Input _button		入力
	 *	@return		bool
	 *  @outhor Riku
	 */
	inline bool IsInput(ActionMapBase::Input _button) const {
		switch (_button.type) {
		case ActionMapBase::InputType::Key:
			return IsKey(_button.input);
		case ActionMapBase::InputType::Mouse:
			return IsMouse(_button.input);
		}
	}
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	ActionMapBase::Input _button		入力
	 *	@return		bool
	 *  @outhor Riku
	 */
	inline bool IsInputUp(ActionMapBase::Input _button) const {
		switch (_button.type) {
		case ActionMapBase::InputType::Key:
			return IsKeyUp(_button.input);
		case ActionMapBase::InputType::Mouse:
			return IsMouseUp(_button.input);
		}
	}
public: // アクションマップ関連
	/*
	 *	各アクションマップの有効状態切り替え
	 *	@param[in]	GameEnum::ActionMap map	切り替えるアクションマップ
	 *  @param[in]	bool setActive			切り替え値
	 *  @outhor Riku
	 */
	inline void SetActionMapIsActive(GameEnum::ActionMap map, bool setActive) {
		actionMaps[map]->isActive = setActive;
	}
	/*
	 *	各アクションマップの入力状態取得
	 *	@param[in]	GameEnum::ActionMap map	取得するアクションマップ
	 *	@return		ActionMapBase::ActionState
	 *  @outhor Riku
	 */
	inline ActionMapBase::ActionState GetInputState(GameEnum::ActionMap map) {
		return actionMaps[map]->state;
	}

};

