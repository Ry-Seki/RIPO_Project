/*
 *	@file	ResultAction_PartTime.h
 *	@author	Seki
 */

#ifndef _RESULT_ACTION_PART_TIME_H_
#define _RESULT_ACTION_PART_TIME_H_

#include "../ResultActionBase.h"

/*
 *	@brief	アルバイトアクションリザルト
 */
class ResultAction_PartTime : public ResultActionBase {
private:
	const int _INCOME = 50;

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
};

#endif // !_RESULT_ACTION_PART_TIME_H_