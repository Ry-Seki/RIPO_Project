/*
 *	@file	PlayerStatusManager.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_MANAGER_H_
#define _PLAYER_STATUS_MANAGER_H_

#include "../../Singleton.h"
#include "../../../Data/PlayerStatusData.h"
#include "../../JSON.h"

class PlayerStatusManager : public Singleton<PlayerStatusManager> {
	friend class Singleton<PlayerStatusManager>;

private:
	PlayerStatusData playerStatus;		// プレイヤーステータスデータ
	int HPLv;
	int staminaLv;
	int strengthLv;
	int resistTimeLv;

private:
	/*
	 *	コンストラクタ
	 */
	PlayerStatusManager() = default;
	/*
	 *	デストラクタ
	 */
	~PlayerStatusManager() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	ロード済みデータのセット (コールバック)
	 *  @param[in]	const JSON setJSON	設定するJSONデータ
	 */
	void SetupData(const JSON setJSON);
	
public:

};

#endif // !_PLAYER_STATUS_MANAGER_H_