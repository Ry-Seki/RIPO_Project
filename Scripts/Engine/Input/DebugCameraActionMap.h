/*
 *	@file	DebugCameraActionMap.h
 *	@author	Riku
 */
#ifndef _DEBUGCAMERAACTIONMAP_H_
#define _DEBUGCAMERAACTIONMAP_H_

#include "ActionMapBase.h"

/*
 *	@brief	デバックカメラ操作のActionMap
 */
class DebugCameraActionMap : public ActionMapBase {
public:
	DebugCameraActionMap() = default;
	~DebugCameraActionMap() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
};

#endif // !_DEBUGCAMERAACTIONMAP_H_
