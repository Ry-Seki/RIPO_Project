/*
 *	@file	StageObjectBase.h]
 *  @author	kuu
 */

#ifndef _STAGEOBJECTBASE_H_
#define _STAGEOBJECTBASE_H_

#include "../../Component/Component.h"
#include "../../VecMath.h"

class StageObjectBase : public Component {
private:
	int ID = -1;
	int masterID = -1;
	int nameID = -1;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

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

	/*
	 *	@function	GetPosition
	 *	@brief		位置の取得
	 *	@return		Vector3
	 */
	inline const Vector3& GetPosition() const {
		return position;
	}
	/*
	 *	@function	SetPosition
	 *	@brief		位置の変更
	 *  @param		Vector3& setValue
	 */
	inline void SetPosition(Vector3& setValue) {
		position = setValue;
	}
	/*
	 *	@function	GetRotation
	 *	@brief		角度の取得
	 *	@return		Vector3
	 */
	inline const Vector3& GetRotation() const {
		return rotation;
	}
	/*
	 *	@function	SetRotation
	 *	@brief		角度の変更
	 *  @param		Vector3& setValue
	 */
	inline void SetRotation(Vector3& setValue) {
		rotation = setValue;
	}
	/*
	 *	@function	GetScale
	 *	@brief		大きさの取得
	 *	@return		Vector3
	 */
	inline const Vector3& GetScale() const {
		return scale;
	}
	/*
	 *	@function	SetScale
	 *	@brief		大きさの変更
	 *  @param		Vector3& setValue
	 */
	inline void SetScale(Vector3& setValue) {
		scale = setValue;
	}


};

#endif // !_STAGEOBJECTBASE_H_
