/*
 *	@file	CharacterManager.h
 */

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include "../Singleton.h"
#include "../Character/CharacterBase.h"
#include "../Engine.h"

/*
 *	キャラクターの管理クラス
 */
class CharacterManager : public Singleton<CharacterManager>{
private:
	CharacterBaseList createCharacterPtr;
	GameObjectList createObjectList;
	Engine* engine;

private:
	CharacterManager();
	~CharacterManager() = default;

public:
	void Initialize(Engine* setEngine);
	void CreatePlayer(const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
	void CreateCharacter(const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });

};

#endif // !_CHARACTERMANAGER_H_

