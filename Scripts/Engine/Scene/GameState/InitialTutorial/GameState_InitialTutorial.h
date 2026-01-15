/*
 *	@file	GameState_InitialTutorial.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_INITIAL_TUTORIAL_H_
#define _GAME_STATE_INITIAL_TUTORIAL_H_

#include "../GameStateBase.h"

// 前方宣言
class Engine;
/*
 *	@brief	NewGame時のチュートリアル状態
 */
class GameState_InitialTutorial : public GameStateBase {

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理(デバッグ用)
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_GAME_STATE_INITIAL_TUTORIAL_H_