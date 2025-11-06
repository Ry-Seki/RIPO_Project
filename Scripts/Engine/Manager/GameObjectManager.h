/*
 *	@file	GameObjectManager.h
 *  @author	Riku
 */
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include <mutex>

/*
 *	ゲームオブジェクトの管理クラス
 */
class GameObjectManager : public Singleton<GameObjectManager> {
	// フレンド宣言
	friend class Singleton<GameObjectManager>;
private:
	Engine* engine;
	GameObjectList unuseObjectList;
	GameObjectList useObjectList;
	std::mutex unuseMutex;

	const size_t CREATE_OBJECT_COUNT= 64;	// 事前に生成する数

private:
	GameObjectManager();
	~GameObjectManager() = default;

public:
	void Initialize(Engine& setEngine);
	GameObjectPtr GetUnuseObject();
	void ResetObject(GameObjectPtr resetObject);

};

#endif // !_GAMEOBJECTMANAGER_H_
