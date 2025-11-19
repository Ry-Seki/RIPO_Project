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
  *  @note	名前空間での省略可能	using namespace CharacterUtility
  */
namespace CharacterUtility {
	/*
	 *	プレイヤー生成
	 *	@param	name		プレイヤーの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 */
	inline void GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax) {
		CharacterManager::GetInstance().GeneratePlayer(name, position, rotation, AABBMin, AABBMax);
	}

	inline void  GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& capsuleStart,
		const Vector3& capsuleEnd,
		const float& capsuleRadius) {
		CharacterManager::GetInstance().GeneratePlayer(name, position, rotation, capsuleStart, capsuleEnd, capsuleRadius);
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
	inline void GenerateEnemy(
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
	inline void RemoveCharacter(int characterID) {
		CharacterManager::GetInstance().RemoveCharacter(characterID);
	}
	/*
	 *	ポインタ指定のキャラクター削除
	 *  @param[in]	const GameObjectPtr& destroyCharacter
	 */
	inline void RemoveCharacter(const GameObjectPtr& destroyCharacter) {
		if (!destroyCharacter) return;
		CharacterManager::GetInstance().RemoveCharacter(destroyCharacter->ID);
	}
	/*
	 *	ポインタ指定のキャラクター削除
	 *  @param[in]	const GameObjectPtr& destroyCharacter
	 */
	inline void RemoveCharacter(const GameObject* destroyCharacter) {
		if (!destroyCharacter) return;
		CharacterManager::GetInstance().RemoveCharacter(destroyCharacter->ID);
	}
	/*
	 *	全てのキャラクター削除
	 */
	inline void RemoveAllCharacter() {
		CharacterManager::GetInstance().RemoveAllCharacter();
	}


	/*
	 *	キャラクターのオーナーオブジェクトの取得
	 *  @param[in]	const CharacterBasePtr& setCharacter
	 *  @return		GameObject*
	 */
	inline GameObject* GetOwnerObject(const CharacterBasePtr& setCharacter) {
		return CharacterManager::GetInstance().GetCharacterOwner(setCharacter);
	}
	/*
	 *	キャラクターにモデルハンドルをセット
	 *	@param[in]	GameObject* gameObject		セットするモデル
	 *  @param[in]	const int modelHandle		モデルハンドル
	 */
	inline void SetCharacterModel(GameObject* gameObject, const int modelHandle) {
		CharacterManager::GetInstance().SetModelHandle(gameObject, modelHandle);
	}
};
#endif // !_CHARACTER_UTILITY_H_
