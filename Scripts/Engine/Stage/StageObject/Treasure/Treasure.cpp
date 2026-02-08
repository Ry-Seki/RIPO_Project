/*
 *	@file	@Treasure.cpp
 *  @author	oorui & Seki
 */

#include "Treasure.h"
#include "../Scripts/Engine/GameConst.h"
#include "../Scripts/Engine/VecMath.h"
#include "../Scripts/Engine/Manager/EffectManager.h"
#include "../Scripts/Engine/Component/EffectComponent.h"
#include "../../../Component/Character/ArmActionComponent.h"
#include "../../../Component/Character/CharacterUtility.h"
#include "UnTreasureState.h"

using namespace CharacterUtility;
/*
 *	コンストラクタ
 */
Treasure::Treasure()
	: StageObjectBase()
	, isCollected(false)
	, pViewingEffect(nullptr)
	, state(std::make_unique<UnTreasureState>()) {
}
/*
 *	デストラクタ
 */
Treasure::~Treasure() {

}
void Treasure::Start() {
}
/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {
	state->Update(*this, deltaTime);
}


/*
 *	お宝が取られたかどうか
 */
bool Treasure::CollectedCheck() {
	// プレイヤーを取得
	auto player = GetPlayer();
	if (!player) return false;

	// プレイヤーの腕を取得
	auto playerArm = player->GetComponent<ArmActionComponent>();
	if (!playerArm) return false;

	// プレイヤーがお宝を取得したかどうか
	auto lifted = playerArm->GetLiftObject();
	if (!lifted) return false;

	// 自身のIDと一致していれば取得済み
	return lifted->ID == GetOwner()->ID;

}

/*
 *	お宝が取得されたときの処理
 */
void Treasure::OnCollected() {
	// エフェクトが存在すれば停止
	if (pViewingEffect) {
		pViewingEffect->EffectStop();
		pViewingEffect = nullptr;
	}
}

/*
 *	お宝が取得されていないときの処理
 */
void Treasure::UnCollected() {
	// すでに生成されていれば何もしない
	if (pViewingEffect) return;

	// エフェクト再生
	pViewingEffect = EffectManager::GetInstance().Instantiate(
		effectName,
		GetOwner()->position
	);

	// 必要筋力値表示

}

