/*
 *	@file	@Treasure.cpp
 *  @author	oorui & Seki
 */

#include "Treasure.h"
#include "../Scripts/Engine/GameConst.h"
#include "../Scripts/Engine/VecMath.h"
 /*
  *	コンストラクタ
  */
Treasure::Treasure()
	: StageObjectBase()
	, isCollected(false){
}
/*
 *	デストラクタ
 */
Treasure::~Treasure() {

}
/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {
	if (isCollected) return;
}
/*
 *	衝突が起きたときに呼び出される処理
 */
void Treasure::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
}

