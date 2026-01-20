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
	GameObjectPtr unuseObject;
	// リストが空なら生成して返す
	if (unuseObjectList.empty()) {
		unuseObject = engine->Create<GameObject>();
	} else {
		// リストの一番後ろを渡す
		unuseObject = unuseObjectList.back();
		// 渡したらリストから削除
		unuseObjectList.pop_back();
	}
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
		return unuseObject;
	}
	// 空きが無ければ末尾に追加
	useObjectList.push_back(unuseObject);
	// IDを保存
	unuseObject->ID = static_cast<int>(useObjectList.size()) - 1;
	return unuseObject;
}

/*
 *	ID指定のオブジェクト取得
 *	@param	int ID	オブジェクトの識別ID
 */
GameObjectPtr GameObjectManager::GetUseObject(int ID) {
	auto objectID = useObjectList.begin() + ID;
	if (useObjectList.end() <= objectID) return nullptr;
	return useObjectList[ID];
}

/*
 *	名前指定のオブジェクト取得
 *	@param[in]	const std::string& searchName	検索する名前
 *  @author		Seki
 */
std::vector<GameObjectPtr> GameObjectManager::GetObjectByName(const std::string& searchName) {
	std::vector<GameObjectPtr> result;
	for (auto& object : useObjectList) {
		if (!object) continue;
		const std::string& objectTag = object->name;
		// 先頭でのname検索
		if (objectTag.rfind(searchName, 0) == 0) {
			result.push_back(object);
		}
	}
	return result;
}
/*
 *	使用オブジェクトリストの当たり判定の設定
 *	@param[in]	bool setColliderFlag
 *	@author		Seki
 */
void GameObjectManager::SetObjectColliderFlag(bool setColliderFlag) {
	for (auto object : useObjectList) {
		if (!object) continue;
		// AABBコライダーの取得
		auto aabbCollider = object->GetComponent<AABBCollider>();
		if (aabbCollider) {
			aabbCollider->isCollider = setColliderFlag;
		}
		// カプセルコライダーの取得
		auto capsuleCollider = object->GetComponent<CapsuleCollider>();
		if (capsuleCollider) {
			capsuleCollider->isCollider = setColliderFlag;
		}
	}
}

/*
 *	オブジェクトのリセット
 *	@param[in]	GameObject*	resetObject		リセット対象オブジェクト
 */
void GameObjectManager::ResetObject(GameObject* resetObject) {
	if (!resetObject) return;
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// オブジェクトのIDの取得
	int resetObjectID = resetObject->ID;
	// 使用リストから削除
	useObjectList[resetObjectID] = nullptr;
	// 未使用リストに戻る
	ReturnGameObjectList(resetObjectID);
	// オブジェクトのリセット
	resetObject->Destroy();
	resetObject->ResetGameObject();
}
