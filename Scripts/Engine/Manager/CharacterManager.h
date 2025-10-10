/*
 *	@file	CharacterManager.h
 *  @author	Riku
 */

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include "../Singleton.h"
#include "../Character/PlayerComponent.h"
#include "../Engine.h"

/*
 *	キャラクターの管理クラス
 */
class CharacterManager : public Singleton<CharacterManager>{
private:
	GameObjectList useObjectList;
	GameObjectList unuseObjectList;
	CharacterBaseList createCharacterList;
	Engine* engine;

	const int CREATE_OBJECT_MAX;

private:
	CharacterManager();
	~CharacterManager() = default;

public:
	void Initialize(Engine& setEngine);
	void CreatePlayer(const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
	void RemoveCharacter(int characterID);
	CharacterBasePtr GetCharacter(int characterID);
	

};

#endif // !_CHARACTERMANAGER_H_

