/*
 *	@file	WeaponBase.cpp
 *  @author Riku
 */

#include "WeaponBase.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/CameraManager.h"

WeaponBase::WeaponBase()
	: ammoCount(0)
	, ammoCountMax(0)
	, reloadingTime(0.0f)
	, reloadingTimeMax(0)

	, BULLET_NAME("bullet")
	, BULLET_AABB_MIN({ -10, 0, -10 })
	, BULLET_AABB_MAX({ 10, 20, 10 })
{}

/*
 *	èeÇåÇÇ¬èàóù
 */
void WeaponBase::ShotBullet() {
	auto camera = CameraManager::GetInstance().GetCamera();
	BulletManager::GetInstance().GenerateBullet(
		BULLET_NAME, camera->position, camera->rotation,
		BULLET_AABB_MIN, BULLET_AABB_MAX);
}

/*
 *	ÉäÉçÅ[Éh
 */
void WeaponBase::BulletReload() {
	ammoCount = ammoCountMax;
}

