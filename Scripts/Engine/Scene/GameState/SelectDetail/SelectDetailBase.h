/*
 *	@file	SelectDetailBase.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_BASE_H_
#define _SELECT_DETAIL_BASE_H_

/*
 *	@brief	行動の詳細を選択するクラス
 */
class SelectDetailBase {
public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~SelectDetailBase() {}
		
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

#endif // !_SELECT_DETAIL_BASE_H_