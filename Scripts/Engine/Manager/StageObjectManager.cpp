/*
 *	@file	StageObjectManager.cpp
 *	@author kuu
 */
#include "StageObjectManager.h"
#include "GameObjectManager.h"
#include "../Stage/StageObject/ExitPoint.h"

StageObjectManager::StageObjectManager()
	: engine(nullptr) {

}

/*
 *	出口生成
 */
void StageObjectManager::CreateExit(
	const int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& scale) {
	// 未使用のオブジェクト取得
	exitObject = GameObjectManager::GetInstance().GetUnuseObject();
	//GameObjectPtr exitStageObj = exitObject->AddComponent<ExitPoint>();
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
	const Vector3& rotation,
	const Vector3& scale) {

}


/*
 *	ID指定のステージオブジェクト削除
 */
void StageObjectManager::RemoveStageObject(int stageObjectID) {
	// リストから削除
	createStageObjectList[stageObjectID] = nullptr;
}

/*
 *	ID指定のステージオブジェクト取得
 */
StageObjectBasePtr StageObjectManager::GetStageObject(int stageObjectID) {
	return createStageObjectList[stageObjectID];
}
