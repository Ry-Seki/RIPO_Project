/*
 *	@file	WeaopnBase.h
 *  @author Riku
 */

#ifndef _WEAPONBASE_H_
#define _WEAPONBASE_H_

#include "ArmActionComponent.h"

/*
 *	銃のウデの基底クラス
 */
class WeaponBase : public ArmActionComponent {
protected:
	int ammoCount;					// 残弾数
	int ammoCountMax;				// 弾の最大数
	float reloadingTime;			// リロードの残り時間

	const float RELOADING_TIME_MAX;	// リロードに掛かる時間

public:
	WeaponBase();
	virtual ~WeaponBase() = default;

	
};

#endif // !_WEAPONBASE_H_
