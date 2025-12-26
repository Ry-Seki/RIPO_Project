/*
 *	@file	SelectDetail_Dungeon.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_DUNGEON_H_
#define _SELECT_DETAIL_DUNGEON_H_

#include "../SelectDetailBase.h"

/*
 *	@brief	ダンジョンの内容を選択するクラス
 */
class SelectDetail_Dungeon : public SelectDetailBase {

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

};

#endif // !_SELECT_DETAIL_DUNGEON_H_