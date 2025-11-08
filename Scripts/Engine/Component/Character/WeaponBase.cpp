/*
 *	@file	WeaopnBase.cpp
 *  @author Riku
 */

#include "WeaponBase.h"
#include "../../Manager/BulletManager.h"

WeaponBase::WeaponBase()
	: ammoCount(0)
	, ammoCountMax(6)
	, reloadingTime(0.0f)

	, BULLET_NAME("bullet")
	, RELOADING_TIME_MAX(3.0f)
	, BULLET_AABB_MIN({ -10, 0, -10 })
	, BULLET_AABB_MAX({ 10, 20, 10 })
{}

/*
 *	èeÇåÇÇ¬èàóù
 */
void WeaponBase::ShotBullet(Vector3 position, Vector3 rotation) {
	BulletManager::GetInstance().GenerateBullet(
		BULLET_NAME, position, rotation, 
		BULLET_AABB_MIN, BULLET_AABB_MAX);
}

