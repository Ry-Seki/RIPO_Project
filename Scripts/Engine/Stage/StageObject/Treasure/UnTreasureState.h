/*
 *	@file	UnTreasureState.h
 *  @author oorui
 */

#ifndef UNTREASURESTATE_H_
#define UNTREASURESTATE_H_

#include "TreasureState.h"
#include "Treasure.h"
#include "OnTreasureState.h"

class UnTreasureState : public TreasureState {
public:
	void Update(Treasure& treasure, float deltaTime) override {

		// 表示用エフェクトを維持
		treasure.UnCollected();

		// プレイヤーに回収されたら状態遷移
		if (treasure.CollectedCheck()) {
			treasure.ChangeState(new OnTreasureState());
		}
	}
};

#endif // !UNTREASURESTATE_H_
