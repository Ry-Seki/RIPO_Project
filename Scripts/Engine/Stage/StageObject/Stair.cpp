/*
 *	@file	Stair.cpp
 *  @author oorui
 */

#include "Stair.h"
#include "../../GameObject.h"
#include "../../GameConst.h"
#include "../StageObject/StageObjectUtility.h"

 /*
  *	階層移動フラグに変更を加える
  *  @param	self
  *  @param	otger
  */
void Stair::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// フラグの変更
	if (other->GetOwner()->name == GameConst::_CREATE_POSNAME_PLAYER) {
		SetStairMove(true);
		// 移動先フロアIDの取得
		StageObjectUtility::SetMoveStairID(stairID);
	}
}

void Stair::ModelLoad(const int modelHandleBase) {
}
