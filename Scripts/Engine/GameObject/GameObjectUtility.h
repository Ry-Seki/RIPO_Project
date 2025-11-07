/*
 *	@file	GameObjectUtility.h
 *	@author	Seki
 */

#ifndef _GAMEOBJECT_UTILITY_H_
#define _GAMEOBJECT_UTILITY_H_

#include "../Manager/GameObjectManager.h"

/*
 *	ゲームオブジェクト関連実行処理
 */
class GameObjectUtility {
public:
	/*
	 *	未使用オブジェクト取得
	 */
	static GameObjectPtr GetUnuseObject() {
		GameObjectManager::GetInstance().GetUnuseObject();
	}
	/*
	 *	ID指定のオブジェクト取得
	 *	@param[in]	int ID	オブジェクトの識別ID
	 */
	static GameObjectPtr GetUseObject(int ID) {
		GameObjectManager::GetInstance().GetUseObject(ID);
	}
	/*
	 *	オブジェクトのリセット
	 *  @param[in]	GameObjectPtr resetObject
	 */
	static void ResetObject(GameObjectPtr resetObject) {
		GameObjectManager::GetInstance().ResetObject(resetObject);
	}
	/*
	 *	オブジェクトのリセット
	 *	@param[in]	GameObject* resetObject
	 */
	static void ResetObject(GameObject* resetObject) {
		GameObjectManager::GetInstance().ResetObject(resetObject);
	}
};
#endif // !_GAMEOBJECT_UTILITY_H_
