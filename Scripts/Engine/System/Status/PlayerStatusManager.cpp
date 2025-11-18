/*
 *	@file	PlayerStatusManager.cpp
 *	@author	Seki
 */

#include "PlayerStatusManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../GameConst.h"

/*
 *	初期化処理
 */
void PlayerStatusManager::Initialize() {
	auto jsonData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Player/PlayerStatusData.json");
	LoadManager::GetInstance().SetOnComplete([this, jsonData]() {
		SetupData(jsonData->GetData());
	});
}
/*
 *	ロード済みデータのセット (コールバック)
 *  @param[in]	const JSON setJSON	設定するJSONデータ
 */
void PlayerStatusManager::SetupData(const JSON setJSON) {
	for (int i = 0, max = playerStatus->base.Length(); i < max; i++) {
		playerStatus->base[i] = setJSON[GameConst::_STATUS_KEY][GameConst::STATUS_PART[i]];
	}
	for (int i = 0, max = playerStatus->rise.Length(); i < max; i++) {
		playerStatus->rise[i] = setJSON[GameConst::_RASE_KEY][GameConst::STATUS_PART[i]];
	}
}
/*
 *	@brief		プレイヤーのステータス上昇
 *  @param[in]	const int statusPart	上昇するステータス
 *  @param[in]	int setValue = 1		上がった回数
 */
void PlayerStatusManager::AddPlayerStatus(const int statusPart, int setValue) {
	playerStatus->base[statusPart] += playerStatus->rise[statusPart] * setValue;
}
