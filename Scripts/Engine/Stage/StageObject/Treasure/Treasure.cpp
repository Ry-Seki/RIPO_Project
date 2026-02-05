/*
 *	@file	@Treasure.cpp
 *  @author	oorui & Seki
 */

#include "Treasure.h"
#include "../Scripts/Engine/GameConst.h"
#include "../Scripts/Engine/VecMath.h"
#include "../Scripts/Engine/Manager/EffectManager.h"
 /*
  *	コンストラクタ
  */
Treasure::Treasure()
	: StageObjectBase()
	, isCollected(false)
	, pViewingEffect(nullptr) {
}
/*
 *	デストラクタ
 */
Treasure::~Treasure() {

}
void Treasure::Start() {
	//EffectManager::GetInstance().Instantiate(
	//	"treasureViewing",
	//	GetOwner()->position
	//);
}
/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {
	if (isCollected) return;

	// まだエフェクトを生成していなければ生成
	if (pViewingEffect == nullptr) {
		pViewingEffect = EffectManager::GetInstance().Instantiate(
			"treasureViewing",
			GetOwner()->position
		);
	}

}
/*
 *	衝突が起きたときに呼び出される処理
 */
void Treasure::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
}

