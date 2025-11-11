/*
 *	@file	Stair.cpp
 *  @author oorui
 */

#include "Stair.h"
#include "../../GameObject.h"
#include "../../GameConst.h"
 /*
  *	階層移動フラグに変更を加える
  *  @param	self
  *  @param	otger
  */
void Stair::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	Component* aself = self.get();
	Component* aother = other.get();
	// フラグの変更
	if (aother->GetOwner()->name == GameConst::_CREATE_POSNAME_PLAYER) {
		SetStairMove(true);
	}
}

void Stair::ModelLoad(const int modelHandleBase) {
}
