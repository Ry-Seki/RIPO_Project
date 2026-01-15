/*
 *	@file	SelectDetail_PartTime.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_PART_TIME_H_
#define _SELECT_DETAIL_PART_TIME_H_

#include "../SelectDetailBase.h"

#include <memory>
#include <vector>

 // 前方宣言
class MiniGameBase;

/*
 *	@brief	アルバイト内容を選択するクラス
 */
class SelectDetail_PartTime : public SelectDetailBase {



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

#endif // !_SELECT_DETAIL_PART_TIME_H_