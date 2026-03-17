/*
 *	@file	EnemyDataUtility.h
 *	@author	kuu
 */
#ifndef _ENEMYDATAUTILITY_H_
#define _ENEMYDATAUTILITY_H_

#include "../../Manager/EnemyDataManager.h"

namespace EnemyDataUtility {
	/*
	 *	ƒ{ƒX‚Ì‘å‚«‚³Žæ“¾
	 */
	inline Vector3 GetBossScale(int dungeonID) {
		return EnemyDataManager::GetInstance().GetBossScale(dungeonID);
	}
};

#endif // !_ENEMYDATAUTILITY_H_
