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
#include "../../../../Engine/Menu/Dungeon/DungeonTreasureUI.h"
#include "../../../Menu/MenuManager.h"

using namespace CharacterUtility;
/*
 *	コンストラクタ
 */
Treasure::Treasure()
	: StageObjectBase()
	, isCollected(false)
	, pViewingEffect(nullptr)
	, state(std::make_unique<UnTreasureState>())
	, viewRadius(5000.0f) {
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

	// UI削除
	auto menu = MenuManager::GetInstance().GetMenu<DungeonTreasureUI>();
	if (menu) {
		menu->HideTreasureUI();

	}
}

/*
 *	お宝が取得されていないときの処理
 */
void Treasure::UnCollected() {
	// Playerの取得
	auto player = GetPlayer();
	if (!player) return;

	// プレイヤーとお宝の距離
	float distance = Distance(player->position, GetOwner()->position);

	if (distance < viewRadius) {

		// エフェクト再生
		if (!pViewingEffect) {
			pViewingEffect = EffectManager::GetInstance().Instantiate(
				effectName,
				GetOwner()->position
			);
		}

		// UI表示
		auto& menu = MenuManager::GetInstance();
		auto treasureMenu = menu.GetMenu<DungeonTreasureUI>();
		treasureMenu->ShowTreasureUI(treasureID, GetOwner()->position);
		menu.OpenMenu<DungeonTreasureUI>();
	}
	else {

		// エフェクトが存在すれば停止
		if (pViewingEffect) {
			pViewingEffect->EffectStop();
			pViewingEffect = nullptr;
		}

		auto& menu = MenuManager::GetInstance();
		auto treasureMenu = menu.GetMenu<DungeonTreasureUI>();
		treasureMenu->HideTreasureUI();
		menu.CloseMenu(treasureMenu);
	}

}

