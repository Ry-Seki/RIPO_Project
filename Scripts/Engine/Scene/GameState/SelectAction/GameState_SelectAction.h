/*
 *	@file	GameState_SelectAction.h
 *  @author	Seki
 */

#ifndef _GAME_STATE_SELECT_ACTION_H_
#define _GAME_STATE_SELECT_ACTION_H_

#include "../GameStateBase.h"
#include "../../../GameEnum.h"

/*
 *	@brief	行動選択状態
 */
class GameState_SelectAction : public GameStateBase {
private:
	bool inputHandle = false;											

public:
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_GAME_STATE_SELECT_ACTION_H_