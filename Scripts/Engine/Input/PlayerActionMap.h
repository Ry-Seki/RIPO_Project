/*
 *	@file	PlayerActionMap.h
 *	@author	Riku
 */
#ifndef _PLAYER_ACTION_MAP_H_
#define _PLAYER_ACTION_MAP_H_

#include "ActionMapBase.h"

#include <unordered_map>

/*
 *	@brief	プレイヤー操作のActionMap
 */
class PlayerActionMap : ActionMapBase {
public:
	// プレイヤーの行動
	enum class PlayerAction {
		ForwardMove,	// 前移動
		RightMove,		// 右移動
		Run,			// ダッシュ
		Avoid,			// 回避
		Jump,			// ジャンプ
		Shot,			// 射撃
		Lift,			// お宝持ち上げ
	};

	// プレイヤーの行動の状態
	struct PlayerActionState {
		std::unordered_map<PlayerAction, float> axis;
		std::unordered_map<PlayerAction, bool> button;
	};
	PlayerActionState state;

public:
	PlayerActionMap() = default;
	~PlayerActionMap() = default;

public:
	/*
	 *	入力の更新
	 */
	void InputUpdate() override;
};

#endif // !_PLAYER_ACTION_MAP_H_
