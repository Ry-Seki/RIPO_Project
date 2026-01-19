/*
 *	@file	ResultAction_Shop.h
 *	@author	Seki
 */

#ifndef _RESULT_ACTION_SHOP_H_
#define _RESULT_ACTION_SHOP_H_

#include "../ResultActionBase.h"

/*
 *	@brief	ショップアクションのリザルト
 */
class ResultAction_Shop : public ResultActionBase {
public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
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
	/*
	 *	@brief	日にち進行処理
	 */
	void AdvanceDay() override;

};

#endif // !_RESULT_ACTION_SHOP_H_