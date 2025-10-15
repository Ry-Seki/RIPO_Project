/*
 *	@file	StageObjectManager.cpp
 *	@author kuu
 */
#include "StageObjectManager.h"

StageObjectManager::StageObjectManager()
	: engine(nullptr) {

}

/*
 *	初期化
 */
void StageObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
}

/*
 *	お宝生成
 */
void StageObjectManager::CreateTreasure(
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
