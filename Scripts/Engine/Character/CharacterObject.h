/*
 *  @file   CharacterObject.h
 */

#ifndef _CHARACTEROBJECT_H_
#define _CHARACTEROBJECT_H_

#include "../GameObject.h"

class CharacterObject : public GameObject {
private:
	int modelID = -1;	// モデルID

protected:
	// 初期化処理
	void Start() override;
	// 更新処理
	void Update(float deltaTime) override;
	// 描画処理
	void Render() override;
	// 衝突処理
	void OnCollision(const ComponentPtr& self, const ComponentPtr& other) override;
	// 破棄処理
	void OnDestroy() override;
	
public:
	// モデル番号の取得
	inline int GetModelID() const { return modelID; }
	// モデル番号の変更
	inline void SetModelID(int& setValue) { modelID = setValue; }
};
//	別名定義
using CharacterObjectPtr = std::shared_ptr<CharacterObject>;
using CharacterObjectList = std::vector<CharacterObjectPtr>;

#endif // !_CHARACTEROBJECT_H_



