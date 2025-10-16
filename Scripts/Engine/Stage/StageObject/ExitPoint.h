/*
 *	@file	ExitPoint
 *  @author kuu
 */

#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_

#include "StageObjectBase.h"
#include "../../Engine.h"

class ExitPoint : StageObjectBase {
private:


public:
	ExitPoint();
	virtual ~ExitPoint() = default;

public:
	void Update();


};

#endif // !_EXITPOINT_H_
