/*
 *	@file	WeaponBase.cpp
 *  @author Riku
 */

#include "WeaponBase.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/WeaponManager.h"
#include "../../Load/LoadManager.h"
#include "RevolverArm.h"

WeaponBase::WeaponBase()
	: number(GameEnum::Weapon::Invalid)
	, ammoCount(0)
	, ammoCountMax(0)
	, reloadingTime(0.0f)
	, reloadingTimeMax(0)
	, shotCoolTime(0)
    , shotCoolTimeMax(0)

	, BULLET_NAME("bullet")
	, BULLET_AABB_MIN({ -10, 0, -10 })
	, BULLET_AABB_MAX({ 10, 20, 10 }) {
	
}

/*
 *	初期化処理
 */
void WeaponBase::Initialize() {
	WeaponManager::GetInstance().GetCurrentWeapon()->Initialize();
}

/*
 *	更新処理
 */
void WeaponBase::ArmUpdate(float deltaTime, ActionMapBase::ActionState action) {
	WeaponBasePtr weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	weapon->ArmUpdate(deltaTime, action);
	int reload = static_cast<int>(GameEnum::PlayerAction::BulletReload);
	
	// 手動リロード
	if (action.buttonDown[reload])
		weapon->BulletReload();
}

/*
 *	銃を撃つ処理
 */
void WeaponBase::ShotBullet() {
	auto camera = CameraManager::GetInstance().GetCamera();
	BulletManager::GetInstance().GenerateBullet(
		BULLET_NAME, camera->position, camera->rotation,
		BULLET_AABB_MIN, BULLET_AABB_MAX);
}

/*
 *	リロード
 */
void WeaponBase::BulletReload() {
	ammoCount = ammoCountMax;
}

/*
 *	番号取得
 */
GameEnum::Weapon WeaponBase::GetNumber() {
	return number;
}
