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

private:
	StageObjectManager();
	~StageObjectManager() = default;

public:
	/*
	 *	初期化
	 */
	void Initialize(Engine& setEngie);

	/* 
	 *	ID指定のステージオブジェクト取得
	 */
	StageObjectBasePtr GetStageObject(int stageObjectID);


};

#endif // !_STAGEOBJECTMANAGER_H_
