/*
 *	@file	GameStateBase.h
 *  @author Seki
 */

#ifndef _GAME_STATE_BASE_H_
#define _GAME_STATE_BASE_H_

/*
 *	@brief	メインゲームの状態クラス
 */
class GameStateBase {
protected:
	bool isFinished = false;	// 終了フラグ

public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~GameStateBase() {}

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize() {}
	/*
	 *	@brief	準備前処理
	 */
	virtual void Setup() = 0;
	/*
	 *	@brief	更新処理
	 */
	virtual void Update(float deltaTime) = 0;
	/*
	 *	@brief	描画処理
	 */
	virtual void Render() {}
	/*
	 *	@brief	片付け処理
	 */
	virtual void Teardown() {}
	/*
	 *	@brief	ポーズ処理
	 */
	virtual void Pause() {}
	/*
	 *	@brief	再開処理
	 */
	virtual void Resume() {}

public:
	/*
	 *	@brief		終了判定
	 *  @return		bool
	 */
	inline bool IsFinished() { return isFinished; }
};

#endif // !_GAME_STATE_BASE_H_