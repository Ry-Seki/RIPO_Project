/*
 *	@file	@Treasure.cpp
 *  @author	oorui
 */

#include "Treasure.h"

 /*
  *	コンストラクタ
  */
Treasure::Treasure()
	: StageObjectBase(), isCollected(false), modelHandle(-1) {
}

Treasure::~Treasure() {

}

/*
  *	@function	ModelLoad
  *  @brief		モデルの読み込み
  *  @param		const int modelHandleBase
  */
void Treasure::ModelLoad(const int modelHandleBase) {
	// 種類別でモデルを読み込む
	modelHandle = modelHandleBase;

}


/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {
	if (isCollected) return;


	// 当たり判定の更新

}


/*
 *	衝突が起きたときに呼び出される処理
 */
void Treasure::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
}


/*
 *	@function	TreasureHold
 *  @brief		持たれている時の処理
 *  @param		const Vector3& position
 */
void Treasure::TreasureHold(const Vector3& position) {
	

}
