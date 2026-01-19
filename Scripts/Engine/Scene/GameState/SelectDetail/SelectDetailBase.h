/*
 *	@file	SelectDetailBase.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_BASE_H_
#define _SELECT_DETAIL_BASE_H_

// 前方宣言
class GameState_SelectDetail;

/*
 *	@brief	行動の詳細を選択するクラス
 */
class SelectDetailBase {
protected:
	bool isStart = false;
	GameState_SelectDetail* owner = nullptr;

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

public:
	/*
	 *	@brief		オーナーの設定
	 *	@param[in]	GameState_SelectDetail* setOwner
	 */
	inline void SetOwner(GameState_SelectDetail* setOwner) {
		owner = setOwner;
	}
};

#endif // !_SELECT_DETAIL_BASE_H_