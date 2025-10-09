/*
 *	@file	StageObjectBase.h
 */

#ifndef _STAGEOBJECTBASE_H_
#define _STAGEOBJECTBASE_H_

#include "../../Component.h"

class StageObjectBase : public Component {
private:
	int ID = -1;
	int masterID = -1;
	int nameID = -1;

public:
	/*
	 *	@function	GetID
	 *	@brief		IDの取得
	 *	@return		int
	 */
	inline int GetID() const {
		return ID;
	}
	/*
	 *	@function	SetID
	 *	@brief		IDの変更
	 *	@param[in]	int& setValue;
	 */
	inline void GetID(int& setValue) {
		ID = setValue;
	}
	/*
	 *	@function	GetNameID
	 *	@brief		名前IDの取得
	 *	@return		int
	 */
	inline int GetNameID() const {
		return nameID;
	}
	/*
	 *	@function	SetNameID
	 *	@brief		名前IDの変更
	 *	@param[in]	int& setValue
	 */
	inline void SetNameID(int& setValue) {
		nameID = setValue;
	}



};

#endif // !_STAGEOBJECTBASE_H_
