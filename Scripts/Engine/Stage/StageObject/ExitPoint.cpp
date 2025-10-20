/*
 *	@file	ExitPoint.h
 *  @author kuu
 */

#include "ExitPoint.h"
#include "../../Fade/FadeFactory.h"

// 出口の衝突イベント
void ExitPoint::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// 既にプレイヤーと衝突していたら処理しない
	if (exitTriger || other->GetOwner()->name != "player")
		return;

	// 衝突済みにする
	exitTriger = true;

	// フェードさせる
	CreateFade();




}

// モデルの読み込み
void ExitPoint::ModelLoad(const int modelHandleBase) {

}
