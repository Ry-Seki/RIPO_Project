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
	float reloadingTimeMax;			// リロードに掛かる時間

	const std::string BULLET_NAME;	// 弾の名前
	const Vector3 BULLET_AABB_MIN;	// 弾のAABBMin
	const Vector3 BULLET_AABB_MAX;	// 弾のAABBMax

public:
	WeaponBase();
	virtual ~WeaponBase() = default;

public:
	/*
	 *	銃を撃つ処理
	 */
	void ShotBullet();

	/*
	 *	リロード
	 */
	void BulletReload();
	
};

#endif // !_WEAPONBASE_H_
