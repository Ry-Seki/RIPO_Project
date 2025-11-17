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
	int HP;				// HP
	int stamina;		// スタミナ
	int strength;		// 力
	int resistTime;		// ダンジョンに潜れる時間
};
#endif // !_PLAYER_STATUS_DATA_H_