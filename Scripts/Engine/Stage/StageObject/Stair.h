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
	int	stairID = -1;				// 階段のID
	int nextID = -1;				// 次の階層のID

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

public:

	/*
	 *	階段のIDの取得
	 */
	inline int GetStairID()const { return stairID; };

	/*
	 *	階段のIDの変更
	 *  @param	setValue	ID番
	 */
	inline void SetStairID(int setValue) { stairID = setValue; }

	/*
	 *	次の階層IDの取得
	 */
	inline int GetNextID()const { return nextID; }

	/*
	 *	次の階層IDの変更
	 *  @oaram	setValue	ID番
	 */
	inline void SetNextID(int setValue) { nextID = setValue; }

};
#endif // !_STAIR_H_

