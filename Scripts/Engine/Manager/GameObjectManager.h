/*
 *	@file	GameObjectManager.h
 */
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"

class GameObjectManager : public Singleton<GameObjectManager> {
private:
	Engine* engine;
	GameObjectList useObjectList;
	GameObjectList unuseObjectList;

	const int CREATE_OBJECT_MAX = 32;

private:
	GameObjectManager();
	~GameObjectManager() = default;

public:
	void Initialize(Engine& setEngine);
	GameObjectPtr UseObject();

};

#endif // !_GAMEOBJECTMANAGER_H_
