/*
 *	@file	ExitPoint
 *  @author kuu
 */

#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_

#include "StageObjectBase.h"
#include "../../Engine.h"
#include "../Scripts/Engine/Component/EffectComponent.h"
#include "../../../Data/Dungeon/DungeonCreatePosData.h"

#include <memory>
class ExitPoint : public StageObjectBase {
private:
	bool exitTriger = false;					// 脱出フラグ
	bool onTrigger = false;						// 衝突検知フラグ
	std::string effectName = "GoalEffect";		// 再生するエフェクトの名前
	float viewRadius;							// エフェクト表示圏内


	EffectComponent* pViewingEffect;			// 表示用エフェクト
	Vector3 startPos;							// 生成位置の参照データ

public:
	/*
	 *	コンストラクタ
	 */
	ExitPoint();
	~ExitPoint() override;

public:

	/*
	 *	更新
	 */
	void Update(float deltaTime)override;

	/*
	 *	出口の衝突イベント
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

	/*
	 *	モデルの読み込み
	 */
	void ModelLoad(const int modelHandleBase) override;

	/*
	 *	脱出フラグの取得
	 */
	inline bool GetExitTriger() const {
		return exitTriger;
	}

	/*
	 *	脱出フラグの指定
	 */
	void SetExitTrigger(bool setValue) { exitTriger = setValue; }

	/*
	 *	生成位置データの設定
	 *  @param[in]	creator		参照する生成位置データ
	 */
	void SetDungeonCreateData(Vector3 setValue) { startPos = setValue; }

};

#endif // !_EXITPOINT_H_