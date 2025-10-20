/*
 *	@file	CharacterManager.h
 *  @author	Riku
 */

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include "../Singleton.h"
#include "../Component/Character/PlayerComponent.h"


/*
 *	キャラクターの管理クラス
 */
class CharacterManager : public Singleton<CharacterManager>{
	// フレンド宣言	
	friend class Singleton<CharacterManager>;
private:;
	Engine* engine;
	CharacterBaseList createCharacterList;
	GameObjectPtr characterObject;

	const size_t CREATE_CHARACTER_COUNT = 16;	// 事前に生成する数

private:
	CharacterManager();
	~CharacterManager() = default;

private:
	template <typename T>
	CharacterBasePtr CreateCharacter(
		const int setID,
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });

public:
	void Initialize(Engine& setEngine);
	void GeneratePlayer(
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
	void RemoveCharacter(int characterID);
	CharacterBasePtr GetCharacter(int characterID);
	

};

#endif // !_CHARACTERMANAGER_H_

