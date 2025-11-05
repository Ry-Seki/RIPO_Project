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
	Engine* engine;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime);

	/*
	 *	視界処理
	 */
	void Vision();



};


#endif // !_VISIONCOMPONENT_H_
