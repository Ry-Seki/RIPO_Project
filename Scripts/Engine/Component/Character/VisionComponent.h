/*
 *	@file	VisionComponent
 *  @author kuu
 */
#ifndef _VISIONCOMPONENT_H_
#define _VISIONCOMPONENT_H_

#include "../Component.h"
#include "../../RayCast.h"

/*
 *	視界判定クラス
 */
class VisionComponent : public Component {
private:
	//Engine* engine;

public:
	VisionComponent();
	~VisionComponent() = default;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime);

	/*
	 *	視界処理
	 *	param[in]	const Vector3& beholderPos	自身の位置
	 *	param[in]	const Vector3& beholderDir	自身の向き
	 *	param[in]	const Vector3& targetPos	目標の位置
	 *	param[in]	float viewAngle				視野角の半角
	 *	param[in]	float viewDistance			視界の距離
	 */
	bool Vision(const Vector3& beholderPos, const Vector3& beholderDir, const Vector3& targetPos, float viewAngle, float viewDistance);



};


#endif // !_VISIONCOMPONENT_H_
