/*
 *	@file	ExitPoint
 *  @author kuu
 */

#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_

#include "StageObjectBase.h"
#include "../../Engine.h"

class ExitPoint : public StageObjectBase {
private:
	// 脱出フラグ
	bool exitTriger = false;

public:
	/*
	 *	コンストラクタ
	 */
	ExitPoint() = default;
	~ExitPoint() override {};

public:

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

};

#endif // !_EXITPOINT_H_