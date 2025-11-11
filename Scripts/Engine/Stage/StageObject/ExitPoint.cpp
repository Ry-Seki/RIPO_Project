/*
 *	@file	ExitPoint.h
 *  @author kuu & oorui
 */

#include "ExitPoint.h"
#include "../../Fade/FadeFactory.h"
#include "../../GameConst.h"

 // 出口の衝突イベント
void ExitPoint::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// 既にプレイヤーと衝突していたら処理しない
	if (exitTriger || other->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER)
		return;

	// 衝突済みにする
	exitTriger = true;

}

// モデルの読み込み
void ExitPoint::ModelLoad(const int modelHandleBase) {

}
