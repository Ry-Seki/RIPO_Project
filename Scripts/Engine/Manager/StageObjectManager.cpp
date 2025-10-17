/*
 *	@file	StageObjectManager.cpp
 *	@author kuu
 */
#include "StageObjectManager.h"
#include "GameObjectManager.h"
#include "../Stage/StageObject/ExitPoint.h"
#include "../Stage/StageObject/Treasure/Treasure.h"
#include "../Stage/StageObject/StageObjectBase.h"

StageObjectManager::StageObjectManager()
	: engine(nullptr) {

}

/*
 *	ステージオブジェクト生成
 */
template <typename T>
StageObjectBasePtr StageObjectManager::CreateStageObject(
	int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// 未使用のオブジェクト取得
	stageObjectObj = GameObjectManager::GetInstance().GetUnuseObject();
	// 出口オブジェクト生成
	StageObjectBasePtr createStageObj = stageObjectObj->AddComponent<T>();
	// ID設定
	createStageObj->SetID(setID);
	// データのセット
	stageObjectObj->SetObjectData(name, position, rotation);
	// 出口オブジェクトを返す
	return createStageObj;
}

/*
 *	初期化
 */
void StageObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
}

/*
 *	出口生成
 */
void StageObjectManager::GenerateExit(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// リストの要素の数
	int stageObjectListCount = static_cast<int>(createStageObjectList.size());
	// 生成ステージオブジェクトの空きをチェック
	for (int i = 0; i < stageObjectListCount; i++) {
		if (createStageObjectList[i] != nullptr) continue;
		// リストの空きに生成
		createStageObjectList[i] = CreateStageObject<ExitPoint>(i, name, position, rotation);
		return;
	}
	// 空きがなかったら一番後ろに生成
	createStageObjectList.push_back(CreateStageObject<ExitPoint>(0, name, position, rotation));
}

void StageObjectManager::GenerateTreasure(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// リストの要素の数
	int stageObjectListCount = static_cast<int>(createStageObjectList.size());
	// 生成ステージオブジェクトの空きをチェック
	for (int i = 0; i < stageObjectListCount; i++) {
		if (createStageObjectList[i] != nullptr) continue;
		// リストの空きに生成
		createStageObjectList[i] = CreateStageObject<Treasure>(i, name, position, rotation);
		return;
	}
	// 空きがなかったら一番後ろに生成
	createStageObjectList.push_back(CreateStageObject<Treasure>(0, name, position, rotation));
}


/*
 *	ID指定のステージオブジェクト削除
 */
void StageObjectManager::RemoveStageObject(int stageObjectID) {
	// リストから削除
	createStageObjectList[stageObjectID] = nullptr;
	// オブジェクトのリセット
	GameObjectManager::GetInstance().ResetObject(stageObjectObj);
}

/*
 *	ID指定のステージオブジェクト取得
 */
StageObjectBasePtr StageObjectManager::GetStageObject(int stageObjectID) {
	return createStageObjectList[stageObjectID];
}