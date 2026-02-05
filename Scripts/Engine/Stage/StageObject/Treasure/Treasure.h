/*
 *	@file	Treasure.h
 *  @author oorui
 */

#ifndef _TREASURE_H_
#define _TREASURE_H_

#include "../StageObjectBase.h"
#include "../Scripts/Engine/Component/EffectComponent.h"

 /*
  *	お宝クラス
  */
class Treasure : public StageObjectBase {
private:
	int spawnID;				// 生成位置のID
	bool isCollected;			// 取得済みかどうか
	float viewRadius;			// 取得範囲
	EffectComponent* pViewingEffect; // 表示用エフェクト
	

public:
	Treasure();
	~Treasure() override;

public:

	/*
	 *	
	 */
	void Start()override;

	/*
	 *	@function	Update
	 *  @brief		更新
	 *  @param		float deltaTime
	 */
	void Update(float deltaTime)override;

	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:
	/*
	 *	@brief		取得済み判定
	 *  @return		bool
	 */
	bool GetCollected() const { return isCollected; }
	/*
	 *	@brief		スポーンIDの取得
	 *	@return		int
	 */
	int GetSpawnID() const { return spawnID; }
	/*
	 *	@brief		スポーンIDの設定
	 *	@param[in]	const int setID
	 */
	inline void SetSpawnID(const int setID) { spawnID = setID; }
};

#endif // !_TREASURE_H_