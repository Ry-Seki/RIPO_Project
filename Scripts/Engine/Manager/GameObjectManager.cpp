/*
 *	@file	GameObjectManager.cpp
 *  @author	Riku
 */
#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: engine(nullptr) {
}

/*
 *	初期化処理
 */
void GameObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	// はじめに一定数生成
	unuseObjectList.reserve(CREATE_OBJECT_COUNT);
	useObjectList.reserve(CREATE_OBJECT_COUNT);
	for (size_t i = 0; i < CREATE_OBJECT_COUNT; i++) {
		// 未使用オブジェクト生成
		unuseObjectList.push_back(engine->Create<GameObject>());
		// 空の使用オブジェクトリスト生成
		useObjectList.push_back(nullptr);
	}
}
/*
 *	ID指定でのオブジェクトリストに戻す処理
 *  @param[in]	int ID	オブジェクトの識別ID
 *  @author		Seki
 */
void GameObjectManager::ReturnGameObjectList(int ID) {
	GameObjectPtr returnObject = GetUseObject(ID);
	if (!returnObject) return;

	unuseObjectList.push_back(returnObject);
}

/*
 *	未使用オブジェクト取得
 */
GameObjectPtr GameObjectManager::GetUnuseObject() {
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// リストが空なら生成して返す
	if (unuseObjectList.empty()) return engine->Create<GameObject>();
	// リストの一番後ろを渡す
	GameObjectPtr unuseObject = unuseObjectList.back();
	// 渡したらリストから削除
	unuseObjectList.pop_back();
	// そのオブジェクトがすでに破棄済みなら初期化する
	if (unuseObject->IsDestroyed()) unuseObject->ResetDestroy();
	// 使用オブジェクトリストの空きをチェック
	size_t useObjectListCount = useObjectList.size();
	for (size_t i = 0; i < useObjectListCount; i++) {
		if (useObjectList[i] != nullptr) continue;
		// 使用リストに追加
		useObjectList[i] = unuseObject;
		// IDを保存
		unuseObject->ID = i;
		continue;
	}

	return unuseObject;
}

/*
 *	ID指定のオブジェクト取得
 *	@param	int ID	オブジェクトの識別ID
 */
GameObjectPtr GameObjectManager::GetUseObject(int ID) {
	return useObjectList[ID];
}

/*
 *	オブジェクトのリセット
 *  @param[in]	GameObjectPtr resetObject	リセット対象オブジェクト
 */
void GameObjectManager::ResetObject(GameObjectPtr resetObject) {
	if (!resetObject) return;
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// オブジェクトのリセット
	resetObject->Destroy();
	resetObject->OnDestroy();
	resetObject->ResetGameObject();
	// 未使用リストに戻る
	unuseObjectList.push_back(resetObject);
}
/*
 *	オブジェクトのリセット
 *	@param[in]	GameObject*	resetObject		リセット対象オブジェクト
 *  @author		Seki
 */
void GameObjectManager::ResetObject(GameObject* resetObject) {
	if (!resetObject) return;
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// オブジェクトのIDの取得
	int resetObjectID = resetObject->ID;
	// オブジェクトのリセット
	resetObject->Destroy();
	resetObject->OnDestroy();
	resetObject->ResetGameObject();
	// 未使用リストに戻る
	ReturnGameObjectList(resetObjectID);
}
