#pragma once
#include "../Singleton.h"

#include <DxLib.h>

/*
 *	@brief	マウスのボタン
 */
enum class MouseInput {
	LeftClick,		//左クリック
	RightClick,		//右クリック
	MiddleClick,	//中央クリック

	MouseInputMax,					//MouseInputの要素数
};

/*
 *	入力管理クラス
 */
class InputManager : public Singleton<InputManager>{
	// フレンド宣言
	friend class Singleton<InputManager>;
private:	//メンバ変数
	char keyState[256];
	char prevKeyState[256];
	int anyKeyState;
	int mouseInput[static_cast<int>(MouseInput::MouseInputMax)];
	bool nowMouseInput[static_cast<int>(MouseInput::MouseInputMax)], prevInput[static_cast<int>(MouseInput::MouseInputMax)];

public:
	// 入力機の種類 
	enum class InputType {
		Key,	// キーボード
		Mouse,	// マウス
	};

	// 入力
	struct Input {
		InputType type;
		int input;
	};

	// 入力の種類
	enum class BindingType {
		Axis,	// 軸入力
		Button,	// ボタン入力
	};

	// 入力の設定
	struct Binding {
		int action;			// 入力に対する行動
		BindingType type;	// 入力の種類
		Input positive;		// 入力を確認するボタン
		Input negative;		// 入力を確認するボタン(positiveと対をなすもの)
	};

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
		return !prevInput[_mouseButton] && nowMouseInput[_mouseButton]; 
	}
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	int _mouseButton		マウスボタン番号
	 *	@return		bool
	 */
	inline bool IsMouse(int _mouseButton) const {
		return nowMouseInput[_mouseButton]; 
	}
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	int _mouseButton		マウスボタン番号
	 *	@return		bool
	 */
	inline bool IsMouseUp(int _mouseButton) const {
		return prevInput[_mouseButton] && !nowMouseInput[_mouseButton];
	}
public: // マウスとキーの両方入力
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	Input _mouseButton		入力
	 *	@return		bool
	 */
	inline bool IsInputDown(Input _button) const {
		switch (_button.type) {
		case InputType::Key:
			return IsKeyDown(_button.input);
		case InputType::Mouse:
			return IsMouseDown(_button.input);
		}
	}
	/*
	 *	ボタンが押されているかどうか
	 *  @param[in]	Input _button		入力
	 *	@return		bool
	 */
	inline bool IsInput(Input _button) const {
		switch (_button.type) {
		case InputType::Key:
			return IsKey(_button.input);
		case InputType::Mouse:
			return IsMouse(_button.input);
		}
	}
	/*
	 *	ボタンが押されたかどうか
	 *  @param[in]	Input _button		入力
	 *	@return		bool
	 */
	inline bool IsInputUp(Input _button) const {
		switch (_button.type) {
		case InputType::Key:
			return IsKeyUp(_button.input);
		case InputType::Mouse:
			return IsMouseUp(_button.input);
		}
	}

};

