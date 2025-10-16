/*
 *	@file	StageObjectManager.h
 *	@author kuu
 */

#ifndef _STAGEOBJECTMANAGER_H_
#define _STAGEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/StageObject/StageObjectBase.h"

/*
 *	ステージオブジェクトの管理クラス
 */
class StageObjectManager : public Singleton<StageObjectManager> {
	// フレンド宣言
	friend class Singleton<StageObjectManager>;

private:
	Engine* engine;
	StageObjectBaseList createStageObjectList;
	GameObjectPtr exitObject;

private:
	StageObjectManager();
	~StageObjectManager() = default;

private:
	/*
	 *	出口生成
	 */
	void CreateExit(
		const int setID,
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f },
		const Vector3& scale = { 0.0f, 0.0f, 0.0f });

public:
	/*
	 *	初期化
	 */
	void Initialize(Engine& setEngine);

	/*
	 *	出口生成
	 */
	void GenerateExit(
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f },
		const Vector3& scale = { 0.0f, 0.0f, 0.0f });

	/*
	 *	ID指定のステージオブジェクト削除
	 */
	void RemoveStageObject(int stageObjectID);

	/* 
	 *	ID指定のステージオブジェクト取得
	 */
	StageObjectBasePtr GetStageObject(int stageObjectID);


};

#endif // !_STAGEOBJECTMANAGER_H_
