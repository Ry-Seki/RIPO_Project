/*
 *	@file	InActionBase.h
 */

#ifndef _IN_ACTION_BASE_H_
#define _IN_ACTION_BASE_H_

// 前方宣言
class Engine;
class GameState_InAction;

/*
 *	@brief	行動実行処理クラス
 */
class InActionBase {
protected:
	GameState_InAction* owner = nullptr;

public:
	/*
	 *	@brief	デストラク
	 */
	virtual ~InActionBase() {}

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize(Engine& engine) {}
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

public:
	/*
	 *	@brief		オーナーの設定
	 *	@param[in]	GameState_InAction* setOwner
	 */
	inline void SetOwner(GameState_InAction* setOwner) {
		owner = setOwner;
	}

};

#endif // !_IN_ACTION_BASE_H_