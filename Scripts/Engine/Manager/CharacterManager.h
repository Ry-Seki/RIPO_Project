/*
 *	@file	CharacterManager.h
 *  @author	Riku
 */

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include "../Singleton.h"
#include "../Component/Character/PlayerComponent.h"
#include "../Component/Character/EnemyComponent.h"


/*
 *	キャラクターの管理クラス
 */
class CharacterManager : public Singleton<CharacterManager>{
	// フレンド宣言	
	friend class Singleton<CharacterManager>;
private:;
	Engine* engine;
	GameObjectList createCharacterList;

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
	GameObjectPtr CreateCharacter(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

	template <typename T>
	GameObjectPtr CreateCharacter(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& capsuleStart,
		const Vector3& capsuleEnd,
		const float& capsuleRadius);

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
	GameObjectPtr GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

	GameObjectPtr GeneratePlayer(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& capsuleStart,
		const Vector3& capsuleEnd,
		const float& capsuleRadius);

	/*
	 *	エネミー生成
	 *	@param	name		エネミーの名前
	 *	@param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸に置ける最小値
	 *  @param	AABBMax		AABBの各軸に置ける最大値
	 *  @author	kuu
	 */
	GameObjectPtr GenerateEnemy(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);
	/*
	 *	ID指定のキャラクター削除処理
	 */
	void RemoveCharacter(int characterID);
	/*
	 *	全てのキャラクター削除処理
	 *  @author	Seki
	 */
	void RemoveAllCharacter();

public:
	/*
	 *	キャラクターにモデルハンドルをセット
	 *	@param[in]	GameObject* gameObject		セットするモデル
	 *  @param[in]	const int modelHandle		モデルハンドル
	 *  @author		Seki
	 */
	void SetModelHandle(GameObject* gameObject, const int modelHandle);
	/*
	 *	キャラクターのオーナーオブジェクトの取得
	 *  @param[in]	const CharacterBasePtr& setCharacter
	 *  @return		GameObject*
	 *  @author		Seki
	 */
	GameObject* GetCharacterOwner(const CharacterBasePtr& setCharacter) const;

};

#endif // !_CHARACTERMANAGER_H_

