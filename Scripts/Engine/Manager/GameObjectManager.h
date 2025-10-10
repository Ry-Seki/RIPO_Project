/*
 *	@file	GameObjectManager.h
 *  @author	Riku
 */
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include <mutex>

class GameObjectManager : public Singleton<GameObjectManager> {
private:
	Engine* engine;
	GameObjectList unuseObjectList;
	std::mutex unuseMutex;

	const int CREATE_OBJECT_MAX = 32;

private:
	GameObjectManager();
	~GameObjectManager() = default;

public:
	void Initialize(Engine& setEngine);
	GameObjectPtr GetUnuseObject();
	void ResetObject(GameObjectPtr resetObject);

};

#endif // !_GAMEOBJECTMANAGER_H_
