/*
 *	@file	WeaponBase.cpp
 *  @author Riku
 */

#include "WeaponBase.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/WeaponManager.h"
#include "../../Load/LoadManager.h"
#include "../../Scene/Scene.h"
#include "../../GameConst.h"
#include "RevolverArm.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../Load/Audio/LoadAudio.h"
#include "../../Load/LoadManager.h"
#include "../../Audio/AudioUtility.h"

using namespace AudioUtility;
using namespace CharacterUtility;

WeaponBase::WeaponBase()
	: number(GameEnum::Weapon::Invalid)
	, ammoCount(0)
	, ammoCountMax(0)
	, reloadingTime(0.0f)
	, reloadingTimeMax(0)
	, shotCoolTime(0)
    , shotCoolTimeMax(0)
	, reload(false) {	
}

/*
 *	最初のUpdateの直前に呼び出される処理
 */
void WeaponBase::Start() {
	auto shotSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/Shot.mp3");
	auto reloadSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/Reload.mp3");
	LoadManager::GetInstance().SetOnComplete([this, shotSE, reloadSE]() {
		AudioUtility::RegisterSEHandle("shotSE", shotSE->GetHandle());
		AudioUtility::RegisterSEHandle("reloadSE", reloadSE->GetHandle());
		});
}

/*
 *	更新処理
 */
void WeaponBase::ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {
	WeaponBasePtr weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	weapon->WeaponUpdate(deltaTime, action, engine);

	 // 手動リロード開始
	int reload = static_cast<int>(GameEnum::PlayerAction::BulletReload);
	if (action.buttonDown[reload] && weapon->ammoCount != weapon->ammoCountMax) {
		weapon->reload = true;
		PlaySE("reloadSE");
	}

	// リロード
	weapon->BulletReload(deltaTime);
}

/*
 *	銃を撃つ処理
 */
void WeaponBase::ShotBullet(Engine* engine) {
	// 着弾地点を確認
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	Ray ray = { camera->position, ForwardDir(camera->rotation) };
	Scene::RayCastHit hitInfo;
	bool hit = engine->GetCurrentScene()->RayCast(
		ray, hitInfo,
		[this](const ColliderBasePtr& col, float distance) {
			// プレイヤーと自分以外のオブジェクト
			return !col ||
				(col->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER &&
					col->GetOwner()->name != "bullet");
		}
	);
	Vector3 moveDirection = Direction(camera->position, hitInfo.point);
	// ダメージ設定
	float playerStrength = GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus().strength;
	auto weaponNumber = WeaponManager::GetInstance().GetCurrentWeaponNum();
	float defaultDamage = WeaponDataManager::GetInstance().GetWeaponData(weaponNumber).defaultDamage;
	float hitDamage = defaultDamage + (defaultDamage * playerStrength * 0.1f);

	// 弾発射
	BulletManager::GetInstance().BulletShot(
		camera->position, camera->rotation, 
		{ 0.5f, 0.5f, 0.5f }, moveDirection,
		GetPlayer().get(), 10000, hitDamage);

	PlaySE("shotSE");	
}

/*
 *	リロード
 */
void WeaponBase::BulletReload(float deltaTime) {
	if (reload) {
		if (reloadingTime <= reloadingTimeMax) {
			reloadingTime += deltaTime;
		}
		else {
			ammoCount = ammoCountMax;
			reloadingTime = 0.0f;
			reload = false;
		}
	}
}

/*
 *	番号取得
 */
GameEnum::Weapon WeaponBase::GetNumber() {
	return number;
}
