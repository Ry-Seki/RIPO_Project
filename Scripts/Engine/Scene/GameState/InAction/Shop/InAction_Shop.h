/*
 *	@file	InAction_Shop.h
 *	@author	Seki
 */

#ifndef _IN_ACTION_SHOP_H_
#define _IN_ACTION_SHOP_H_

#include "../InActionBase.h"

/*
 *	@brief	ショップ行動実行クラス
 */
class InAction_Shop : public InActionBase {

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
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_IN_ACTION_SHOP_H_