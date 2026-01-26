/*
 *	@file	PlayerStatusManager.cpp
 *	@author	Seki
 */

#include "PlayerStatusManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../GameConst.h"
#include "../../../Data/SaveData.h"

/*
 *	初期化処理
 */
void PlayerStatusManager::Initialize() {
	auto jsonData = LoadManager::GetInstance().LoadResource<LoadJSON>(_PLAYER_STATUS_DATA_PATH);
	LoadManager::GetInstance().SetOnComplete([this, jsonData]() {
		SetupData(jsonData->GetData());
	});
}
/*
 *	ロード済みデータのセット (コールバック)
 *  @param[in]	const JSON& setJSON	設定するJSONデータ
 */
void PlayerStatusManager::SetupData(const JSON& setJSON) {
	for (int i = 0, max = playerStatus->base.Length(); i < max; i++) {
		playerStatus->base[i] = setJSON[GameConst::_STATUS_KEY][GameConst::STATUS_PART[i]];
	}
	for (int i = 0, max = playerStatus->rise.Length(); i < max; i++) {
		playerStatus->rise[i] = setJSON[GameConst::_RASE_KEY][GameConst::STATUS_PART[i]];
	}
	ApplyPlayerStatus();
}
/*
 *	@brief		レベル指定のプレイヤーステータス設定
 *	@param[in]	int statusPart
 *	@param[in]	int setLevel
 */
void PlayerStatusManager::SetPlayerStatus(int statusPart, int setLevel) {
	if (setLevel == 0) return;
	playerStatus->lv[statusPart] = setLevel;
	playerStatus->base[statusPart] += playerStatus->rise[statusPart] * setLevel;
}
/*
 *	@brief		レベル指定のプレイヤーのステータス上昇
 *  @param[in]	int statusPart	上昇するステータス
 *  @param[in]	int setLevel	上がった回数
 */
void PlayerStatusManager::AddPlayerStatus(int statusPart, int setLevel) {
	if (setLevel == 0) return;
	playerStatus->lv[statusPart] += setLevel;
	playerStatus->base[statusPart] += playerStatus->rise[statusPart] * setLevel;
}
/*
 *	@brief		セーブ用ステータスレベルデータの収集
 *	@return		PlayerStatusLevelData
 */
PlayerStatusLevelData PlayerStatusManager::GetSaveData() const {
	PlayerStatusLevelData data{};
	data.hpLevel = playerStatus->lv.HP;
	data.staminaLevel = playerStatus->lv.stamina;
	data.strengthLevel = playerStatus->lv.strength;
	data.resistTimeLevel = playerStatus->lv.resistTime;
	return data;
}
/*
 *	@brief		セーブデータからステータスレベルを設定
 *	@param[in]	const PlayerStatusLevelData& data
 */
void PlayerStatusManager::ApplyLoadData(const PlayerStatusLevelData& data) {
	playerStatus->lv.HP = data.hpLevel;
	playerStatus->lv.stamina = data.staminaLevel;
	playerStatus->lv.strength = data.strengthLevel;
	playerStatus->lv.resistTime = data.resistTimeLevel;
}
/*
 *	@brief		セーブデータからステータスレベルを設定
 */
void PlayerStatusManager::ApplyPlayerStatus() {
	for (int i = 0, max = playerStatus->base.Length(); i < max; i++) {
		SetPlayerStatus(i, playerStatus->lv[i]);
	}
}
