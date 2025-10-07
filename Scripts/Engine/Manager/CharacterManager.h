/*
 *	@file	CharacterManager.h
 */

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include "../Singleton.h"
#include "../GameObject.h"

/*
 *	キャラクターの管理クラス
 */
class CharacterManager : public Singleton<CharacterManager>{
private:
	GameObjectList createObjectList;

private:
	CharacterManager();
	~CharacterManager() = default;

public:
	
	void CreateCharacter();

};

#endif // !_CHARACTERMANAGER_H_

