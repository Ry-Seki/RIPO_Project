/*
 *	@file	InActionBase.h
 */

#ifndef _IN_ACTION_BASE_H_
#define _IN_ACTION_BASE_H_

/*
 *	@brief	行動実行処理クラス
 */
class InActionBase {

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

};

#endif // !_IN_ACTION_BASE_H_