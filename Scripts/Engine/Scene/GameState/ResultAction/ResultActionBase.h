/*
 *	@brief	ResultActionBase.h
 *	@author	Seki
 */

#ifndef _RESULT_ACTION_BASE_H_
#define _RESULT_ACTION_BASE_H_

// 前方宣言
class GameState_ResultAction;

/*
 *	@brief	アクションリザルト基底クラス
 */
class ResultActionBase {
protected:
	GameState_ResultAction* owner = nullptr;		// オーナークラス

public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~ResultActionBase() {}

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
	 *	@param[in]	GameState_ResultAction* setOwner
	 */
	inline void SetOnwer(GameState_ResultAction* setOwner) {
		owner = setOwner;
	}
};

#endif // !_RESULT_ACTION_BASE_H_