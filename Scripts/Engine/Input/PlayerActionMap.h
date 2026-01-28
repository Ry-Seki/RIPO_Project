/*
 *	@file	PlayerActionMap.h
 *	@author	Riku
 */
#ifndef _PLAYER_ACTION_MAP_H_
#define _PLAYER_ACTION_MAP_H_

#include "ActionMapBase.h"

#include <unordered_map>

/*
 *	@brief	ƒvƒŒƒCƒ„[‘€ì‚ÌActionMap
 */
class PlayerActionMap : public ActionMapBase {
public:
	PlayerActionMap() = default;
	~PlayerActionMap() override  = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize() override;
};

#endif // !_PLAYER_ACTION_MAP_H_
