/*
 *	@file	WeaopnBase.h
 *  @author Riku
 */

#ifndef _WEAPONBASE_H_
#define _WEAPONBASE_H_

#include "../Component.h"

/*
 *	銃のウデの基底クラス
 */
class WeaponBase : public Component {
protected:
	int ammoCount;		// 残弾数
	int ammoCountMax;	// 弾の最大数


};

#endif // !_WEAPONBASE_H_
