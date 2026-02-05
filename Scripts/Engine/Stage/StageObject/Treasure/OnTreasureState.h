/*
 *	@file	OnTreasureState.h
 *  @author oorui
 */

#ifndef ONTREASURESTATE_H_
#define ONTREASURESTATE_H_

#include "TreasureState.h"
#include "Treasure.h"
 /*
  *	‚¨•ó‚ª‰ñû‚³‚ê‚½ó‘Ô
  */
class OnTreasureState : public TreasureState {
public:
	void Update(Treasure& treasure, float deltaTime) override {

		// ‰‰ñ‚¾‚¯‰ñûˆ—
		treasure.OnCollected();

	}
};
#endif // !ONTREASURESTATE_H_
