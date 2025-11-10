/*
 *	@file	GameObjectUtility.h
 *	@author	Seki
 */

#ifndef _GAMEOBJECT_UTILITY_H_
#define _GAMEOBJECT_UTILITY_H_

#include "../Manager/GameObjectManager.h"

/*
 *	ゲームオブジェクト関連実行処理
 *  @note	名前空間での省略可能	using namespace GameObjectUtility
 */
namespace GameObjectUtility {
	/*
	 *	未使用オブジェクト取得
	 */
	inline GameObjectPtr GetUnuseObject() {
		return GameObjectManager::GetInstance().GetUnuseObject();
	}
	/*
	 *	ID指定のオブジェクト取得
	 *	@param[in]	int ID	オブジェクトの識別ID
	 */
	inline GameObjectPtr GetUseObject(int ID) {
		return GameObjectManager::GetInstance().GetUseObject(ID);
	}
	/*
	 *	オブジェクトのリセット
	 *  @param[in]	GameObjectPtr resetObject
	 */
	inline void ResetObject(GameObjectPtr resetObject) {
		GameObjectManager::GetInstance().ResetObject(resetObject);
	}
	/*
	 *	オブジェクトのリセット
	 *	@param[in]	GameObject* resetObject
	 */
	inline void ResetObject(GameObject* resetObject) {
		GameObjectManager::GetInstance().ResetObject(resetObject);
	}
};
#endif // !_GAMEOBJECT_UTILITY_H_
