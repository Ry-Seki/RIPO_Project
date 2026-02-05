/*
 *	@file	TreasureState.h
 *  @author oorui
 */

#ifndef TREASURESTATE_H_
#define TREASURESTATE_H_

 /*
  *	お宝の状態基底クラス
  */
class TreasureState {
public:
	virtual ~TreasureState() = default;

	/*
	 *	状態ごとの更新処理
	 */
	virtual void Update(class Treasure& treasure, float deltaTime) = 0;
};

#endif // !TREASURESTATE_H_

