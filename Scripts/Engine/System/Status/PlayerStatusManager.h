/*
 *	@file	PlayerStatusManager.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_MANAGER_H_
#define _PLAYER_STATUS_MANAGER_H_

#include "../Singleton.h"
#include "../../Data/PlayerStatusData.h"

class PlayerStatusManager : public Singleton<PlayerStatusManager> {
	friend class Singleton<PlayerStatusManager>;

private:
	PlayerStatusData playerStatus;

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
	 */
	void SetupData();
	
public:

};

#endif // !_PLAYER_STATUS_MANAGER_H_