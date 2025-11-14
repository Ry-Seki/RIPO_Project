/*
 *	@file	PlayerStatusData.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_DATA_H_
#define _PLAYER_STATUS_DATA_H_

/*
 *	プレイヤーのステータスデータ
 */
struct PlayerStatusData {
	int HPLv;			// HPレベル
	int staminaLv;		// スタミナレベル
	int strengthLv;		// 力レベル
	int resistTimeLv;	// ダンジョンに潜れる時間レベル
};
#endif // !_PLAYER_STATUS_DATA_H_