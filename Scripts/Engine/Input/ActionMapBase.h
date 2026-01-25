/*
 *	@file	ActionMapBase.h
 *  @author Riku
 */

#ifndef _ACTION_MAP_BASE_H_
#define _ACTION_MAP_BASE_H_

#include <unordered_map>

/*
 *	アクションマップの基底クラス
 */
class ActionMapBase {
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
	// 行動の状態
	struct ActionState {
		std::unordered_map<int, float> axis;
		std::unordered_map<int, bool> button;
	};

	std::vector<Binding> bindings;	// アクションマップの入力設定
	ActionState state;				// 各アクションマップの入力状態
	bool isActive;					// アクションマップの有効状態


public:
	ActionMapBase() = default;
	~ActionMapBase() = default;

public:
	/*
	 *	軸入力設定
	 *	@param	int	action		行動設定
	 *	@param	Input positive	+方向の入力ボタン
	 *	@param	Input negative	-方向の入力ボタン
	 */
	void AddAxis(int action, Input positive, Input negative) {
		bindings.push_back({action, BindingType::Axis, positive, negative});
	}

	/*
	 *	ボタン入力設定
	 *  @param  ActionMap map	アクションマップ
	 *	@param	int action		行動設定
	 *	@param	Input input		ボタン設定ボタン
	 */
	void AddButton(int action, Input input) {
		bindings.push_back({action, BindingType::Button, input, input});
	}

	/*
	 *	入力の更新
	 */
	void InputUpdate();

	/*
	 *	入力状態のリセット
	 */
	void InputReset() {
		state.axis.clear();
		state.button.clear();
	}

public:
	/*
	 *	初期化処理
	 */
	virtual void Initialize() = 0;
};

#endif // !_ACTION_MAP_BASE_H_