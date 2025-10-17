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


public:
	ExitPoint() = default;
	~ExitPoint() override {};

public:
	void Update();


};

#endif // !_EXITPOINT_H_
