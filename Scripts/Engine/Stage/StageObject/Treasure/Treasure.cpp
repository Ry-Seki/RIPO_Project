/*
 *	@file	@Treasure.cpp
 *  @author	oorui
 */

#include "Treasure.h"

Treasure::Treasure() : StageObjectBase() {
	
}

Treasure::~Treasure() {

}

/*
  *	@function	ModelLoad
  *  @brief		モデルの読み込み
  *  @param		const int modelHandleBase
  */
void Treasure::ModelLoad(const int modelHandleBase) {
}


/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {


}


/*
 *	衝突が起きたときに呼び出される処理
 */
void Treasure::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
}
