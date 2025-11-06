/*
 *	@file	CharacterUtility.h
 *	@author	Seki
 */

#ifndef _CHARACTER_UTILITY_H_
#define _CHARACTER_UTILITY_H_

#include "../../Manager/CharacterManager.h"
#include "../../VecMath.h"
#include <string>

/*
 *	キャラクター関連実行処理
 */
class CharacterUtility {
public:
	/*
	 *	プレイヤー生成
	 *	@param	name		プレイヤーの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 */
	static void GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax) {
		CharacterManager::GetInstance().GeneratePlayer(name, position, rotation, AABBMin, AABBMax);
	}

	/*
	 *	エネミー生成
	 *	@param	name		エネミーの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 *  @author	kuu
	 */
	static void GenerateEnemy(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax) {
		CharacterManager::GetInstance().GenerateEnemy(name, position, rotation, AABBMin, AABBMax);
	}
	/*
	 *	ID指定のキャラクター削除
	 *  @param[in]	int characterID
	 */
	static void RemoveCharacter(int characterID) {
		CharacterManager::GetInstance().RemoveCharacter(characterID);
	}
	/*
	 *	ポインタ指定のキャラクター削除
	 *  @param[in]	const CharacterBasePtr& destroyCharacter
	 */
	static void RemoveCharacter(const CharacterBasePtr& destroyCharacter) {
		if (!destroyCharacter) return;
		int characterID = destroyCharacter->GetID();
		CharacterManager::GetInstance().RemoveCharacter(characterID);
	}
	/*
	 *	全てのキャラクター削除
	 */
	static void RemoveAllCharacter() {
		CharacterManager::GetInstance().RemoveAllCharacter();
	}
	/*
	 *	キャラクターのオーナーオブジェクトの取得
	 *  @param[in]	const CharacterBasePtr& setCharacter
	 *  @return		GameObject*
	 *  @author		Seki
	 */
	static GameObject* GetOwnerObject(const CharacterBasePtr& setCharacter) {
		return CharacterManager::GetInstance().GetCharacterOwner(setCharacter);
	}
};
#endif // !_CHARACTER_UTILITY_H_
