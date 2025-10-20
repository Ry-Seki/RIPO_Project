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
	bool exitTriger = false;

public:
	ExitPoint() = default;
	~ExitPoint() override {};

public:
	// 出口の衝突イベント
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

	// モデルの読み込み
	void ModelLoad(const int modelHandleBase) override;
};

#endif // !_EXITPOINT_H_