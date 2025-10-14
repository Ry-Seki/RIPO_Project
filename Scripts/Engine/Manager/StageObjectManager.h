/*
 *	@file	StageObjectManager.h
 *	@author kuu
 */

#ifndef _STAGEOBJECTMANAGER_H_
#define _STAGEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"

/*
 *	ステージオブジェクトの管理クラス
 */
class StageObjectManager : public Singleton<StageObjectManager> {
	// フレンド宣言
	friend class Singleton<StageObjectManager>;

private:
	Engine* enigne;


private:
	StageObjectManager();
	~StageObjectManager() = default;

};

#endif // !_STAGEOBJECTMANAGER_H_
