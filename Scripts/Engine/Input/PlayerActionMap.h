/*
 *	@file	PlayerActionMap.h
 *	@author	Riku
 */
#ifndef _PLAYERACTIONMAP_H_
#define _PLAYERACTIONMAP_H_

#include "ActionMapBase.h"

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

#endif // !_PLAYERACTIONMAP_H_
