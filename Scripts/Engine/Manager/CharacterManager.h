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
	/*
	 *	キャラクター生成
	 *	@param	setID		キャラクターの識別ID
	 *	@param	name		キャラクターの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 */
	template <typename T>
	CharacterBasePtr CreateCharacter(
		const int setID,
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

public:
	void Initialize(Engine& setEngine);
	/*
	 *	プレイヤー生成
	 *	@param	name		プレイヤーの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 */
	void GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);
	void RemoveCharacter(int characterID);
	CharacterBasePtr GetCharacter(int characterID);
	

};

#endif // !_CHARACTERMANAGER_H_

