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

// 前方宣言
struct PlayerStatusLevelData;

/*
 *	プレイヤーのステータス管理クラス
 */
class PlayerStatusManager : public Singleton<PlayerStatusManager> {
	friend class Singleton<PlayerStatusManager>;

private:
	std::shared_ptr<PlayerStatusData> playerStatus;		// プレイヤーステータスデータ

	static constexpr const char* _PLAYER_STATUS_DATA_PATH = "Data/Player/PlayerStatusData.json";

private:
	/*
	 *	コンストラクタ
	 */
	PlayerStatusManager() : playerStatus(std::make_shared<PlayerStatusData>()){};
	/*
	 *	デストラクタ
	 */
	~PlayerStatusManager() override = default;

private:
	/*
	 *	@param[in]	ロード済みデータのセット (コールバック)
	 *  @param[in]	const JSON& setJSON	設定するJSONデータ
	 */
	void SetupData(const JSON& setJSON);
	/*
	 *	@brief		セーブデータからステータスレベルを設定
	 */
	void ApplyPlayerStatus();

public:
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief		レベル指定のプレイヤーのステータス上昇
	 *  @param[in]	const int statusPart	上昇するステータス
	 *  @param[in]	int setValue = 1		上がった回数
	 */
	void AddPlayerStatus(const int statusPart, int setValue = 1);
	/*
	 *	@brief		セーブ用ステータスレベルデータの収集
	 *	@return		PlayerStatusLevelData
	 */
	PlayerStatusLevelData GetSaveData() const;
	/*
	 *	@brief		セーブデータからステータスレベルを設定
	 *	@param[in]	const PlayerStatusLevelData& data
	 */
	void ApplyLoadData(const PlayerStatusLevelData& data);


public:
	/*
	 *	@brief	プレイヤーのステータスデータの取得
	 *	@return	PlayerStatusData*
	 */
	inline PlayerStatusData* GetPlayerStatusData() const { return playerStatus.get(); }

};

#endif // !_PLAYER_STATUS_MANAGER_H_