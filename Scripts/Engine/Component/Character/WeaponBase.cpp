/*
 *	@file	WeaponBase.cpp
 *  @author Riku
 */

#include "WeaponBase.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/CameraManager.h"
#include "../../Load/LoadManager.h"
#include "RevolverArm.h"

WeaponBase::WeaponBase()
	: ammoCount(0)
	, ammoCountMax(0)
	, reloadingTime(0.0f)
	, reloadingTimeMax(0)

	, BULLET_NAME("bullet")
	, BULLET_AABB_MIN({ -10, 0, -10 })
	, BULLET_AABB_MAX({ 10, 20, 10 }) {
	
}

/*
 *	最初のUpdateの直前に呼び出される処理
 */
void WeaponBase::Start() {
	json = LoadManager::GetInstance().LoadResource<LoadJSON>(WEAPON_DATA_PATH)->GetData();
	weapons[GameEnum::Weapon::Revolver] = std::make_shared<RevolverArm>();
	// 初期設定はリボルバー
	currentWeapon = weapons[GameEnum::Weapon::Revolver];
}

/*
 *	初期化処理
 */
void WeaponBase::Initialize() {
	currentWeapon->Initialize();
}

/*
 *	更新処理
 */
void WeaponBase::ArmUpdate(float deltaTime, ActionMapBase::ActionState action) {
	currentWeapon->ArmUpdate(deltaTime, action);
	int reload = static_cast<int>(GameEnum::PlayerAction::BulletReload);
	
	// 手動リロード
	if (action.buttonDown[reload])
		currentWeapon->BulletReload();
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
