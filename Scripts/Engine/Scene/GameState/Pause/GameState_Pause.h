/*
 *	@file	GameState_Pause.h
 *  @author	Seki
 */

#ifndef _GAME_STATE_PAUSE_H_
#define _GAME_STATE_PAUSE_H_

#include "../GameStateBase.h"

/*
 *	@brief	ゲーム中断状態
 */
class GameState_Pause : public GameStateBase {

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
#endif // !_GAME_STATE_PAUSE_H_