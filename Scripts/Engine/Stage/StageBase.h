/*
 *	@file	StageBase.h
 *  @author kuu
 */

#include <DxLib.h>

#ifndef _STAGEBASE_H_
#define _STAGEBASE_H_

class StageBase {
private:


public:
	// コンストラクタ
	StageBase() = default;
	virtual ~StageBase() = default;

protected:
	// 片付け処理
	void Clean(int MHandle) {
		MV1SetVisible(MHandle, false);
	}

public:
	// 更新処理
	virtual void OnLoad() {};
};

#endif // !_STAGEBASE_H_