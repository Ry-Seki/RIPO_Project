/*
 *	@file	PlayerStatusManager.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_MANAGER_H_
#define _PLAYER_STATUS_MANAGER_H_

#include "../../Singleton.h"
#include "../../../Data/PlayerStatusData.h"
#include "../../JSON.h"

#include <memory>

/*
 *	プレイヤーのステータス管理クラス
 */
class PlayerStatusManager : public Singleton<PlayerStatusManager> {
	friend class Singleton<PlayerStatusManager>;

private:
	std::shared_ptr<PlayerStatusData> playerStatus;		// プレイヤーステータスデータ

private:
	/*
	 *	コンストラクタ
	 */
	PlayerStatusManager() : playerStatus(std::make_shared<PlayerStatusData>()){};
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
	 *	@param[in]	ロード済みデータのセット (コールバック)
	 *  @param[in]	const JSON setJSON	設定するJSONデータ
	 */
	void SetupData(const JSON setJSON);
	/*
	 *	@brief		プレイヤーのステータス上昇
	 *  @param[in]	const int statusPart	上昇するステータス
	 *  @param[in]	int setValue = 1		上がった回数
	 */
	void AddPlayerStatus(const int statusPart, int setValue = 1);

public:
	/*
	 *	@brief	プレイヤーのステータスデータの取得
	 *	@return	PlayerStatusData*
	 */
	inline PlayerStatusData* GetPlayerStatusData() const { return playerStatus.get(); }

};

#endif // !_PLAYER_STATUS_MANAGER_H_