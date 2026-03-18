/*
 *	@file	Respawn.h
 *  @author oorui
 */

#ifndef _RESPAWN_H_
#define _RESPAWN_H_

#include "StageObjectBase.h"
#include "../../JSON.h"

class Respawn :public StageObjectBase {
private:
	int respawnID = 0;	// 自身のID

public:
	Respawn() = default;
	~Respawn() = default;

public:
	/*
	 *	リスポーンのIDの取得
	 */
	inline int GetRespawnID()const { return respawnID; }

	/*
	 *	リスポーンIDの設定
	 */
	inline void SetRespawnID(int setValue) { respawnID = setValue; }


};

#endif // !_RESPAWN_H_
