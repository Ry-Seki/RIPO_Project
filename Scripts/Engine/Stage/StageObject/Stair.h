/*
 *	@file	Stair.h
 *  @author oorui
 */

#ifndef _STAIR_H_
#define _STAIR_H_

#include "StageObjectBase.h"

class Stair : public StageObjectBase {
private:
	bool isStairMoved = false;		// 階層移動フラグ

public:
	Stair() :isStairMoved(false) {};		// コンストラクタ
	~Stair() override {};		// デストラクタ
public:

	/*
	 *	階層移動フラグに変更を加える
	 *  @param	self
	 *  @param	otger
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)override;

public:

	/*
	 *	階層移動フラグの取得
	 *  @return isStairMoved
	 */
	inline bool GetStairMove()const {
		return isStairMoved;
	}
	// モデルの読み込み
	void ModelLoad(const int modelHandleBase) override;
	/*
	 *	階段移動のフラグに変更を加える
	 *  @param	setValue
	 *  @tips	true  = 触れた
	 *  @tips	false = 触れてない
	 */
	void SetStairMove(bool setValue) { isStairMoved = setValue; }


};
#endif // !_STAIR_H_

