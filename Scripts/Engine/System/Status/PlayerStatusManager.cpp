/*
 *	@file	PlayerStatusManager.cpp
 *	@author	Seki
 */

#include "PlayerStatusManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"

/*
 *	初期化処理
 */
void PlayerStatusManager::Initialize() {
	auto jsonData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Player/PlayerStatusData.json");
	LoadManager::GetInstance().SetOnComplete([]() {

	});
}
/*
 *	ロード済みデータのセット (コールバック)
 *  @param[in]	const JSON setJSON	設定するJSONデータ
 */
void PlayerStatusManager::SetupData(const JSON setJSON) {

}

