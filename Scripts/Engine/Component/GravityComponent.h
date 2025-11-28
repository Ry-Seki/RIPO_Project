/*
 *	@file	GravityComponent.h
 *  @author Riku
 */

#ifndef _GRAVITYCOMPONENT_H_
#define _GRAVITYCOMPONENT_H_

#include "Component.h"
#include "../VecMath.h"

 /*
  *	重力
  */
class GravityComponent : public Component {
private:
	float fallSpeed;					// 落下速度

	const float FALL_SPEED_MAX;			// 最大落下速度
	const float GRAVITY_ACCELERATION;	// 重力加速度

	bool isGravity = false;				// 重力を掛けるかどうか
public:
	bool hitGroundingFrag = false;		// 接地判定
public:
	GravityComponent();
	~GravityComponent() = default;

public:
	void Update(float deltaTime) override;

public:
	/*
	 *	落下速度に指定した値を加える
	 *  @param	float	addValue	増加値
	 */
	void AddFallSpeed(float addValue) {
		fallSpeed += addValue;
	}

public:
	/*
	 *	接地判定の変更
	 *  @param	bool _setValue
	 *  @author oorui
	 */
	void SetGroundingFrag(bool _setValue) { hitGroundingFrag = _setValue; }


	/*
	 *	接地判定の取得
	 *  @author oorui
	 */
	bool GetGroundingFrag()const { return hitGroundingFrag; }

	/*
	 * 落下速度の取得
	 * @author oorui
	 */
	float GetFallSpeed()const { return fallSpeed; }

	/*
	 *	重力を掛けるフラグの変更
	 *  @param	setValue	重力を掛けるフラグ
	 *  @author oorui
	 */
	void SetGravity(const bool setValue) { isGravity = setValue; }

	/*
	 *	重力がかかっているかどうか取得する
	 *  @author oorui
	 */
	bool GetGravity()const { return isGravity; }
};

#endif // !_GRAVITYCOMPONENT_H_
