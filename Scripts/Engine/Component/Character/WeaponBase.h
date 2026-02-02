/*
 *	@file	WeaopnBase.h
 *  @author Riku
 */

#ifndef _WEAPONBASE_H_
#define _WEAPONBASE_H_

#include "ArmBase.h"
#include "../../GameEnum.h"
#include "../../VecMath.h"
#include "../../Load/JSON/LoadJSON.h"
#include <string>

 // 別名定義
class WeaponBase;
using WeaponBasePtr = std::shared_ptr<WeaponBase>;
/*
 *	銃のウデの基底クラス
 */
class WeaponBase : public ArmBase {
private :
	JSON json;
protected:
	WeaponBasePtr currentWeapon;									// 現在設定されている銃
	std::unordered_map<GameEnum::Weapon, WeaponBasePtr> weapons;	// 設定可能な銃達
	int ammoCount;					// 残弾数
	int ammoCountMax;				// 弾の最大数
	float reloadingTime;			// リロードの残り時間
	float reloadingTimeMax;			// リロードに掛かる時間

	const std::string BULLET_NAME;	// 弾の名前
	const Vector3 BULLET_AABB_MIN;	// 弾のAABBMin
	const Vector3 BULLET_AABB_MAX;	// 弾のAABBMax

	static constexpr const char* WEAPON_DATA_PATH = "Data/Player/Weapon/WeaponData.json";
public:
	WeaponBase();
	virtual ~WeaponBase() = default;

public:
	/*
	 *	最初のUpdateの直前に呼び出される処理
	 */
	void Start() override;
	/*
	 *	初期化処理
	 */
	virtual void Initialize() override;
	/*
	 *	更新処理
	 */
	virtual void ArmUpdate(float deltaTime, ActionMapBase::ActionState action) override;

public:
	/*
	 *	使用する武器の設定
	 */
	inline void SetCurrentWeapon(GameEnum::Weapon setWeapon) {
		if (currentWeapon == weapons[setWeapon])
			return;
		currentWeapon = weapons[setWeapon];
		currentWeapon->Initialize();
	}

protected:
	/*
	 *	銃を撃つ処理
	 */
	void ShotBullet();

	/*
	 *	リロード
	 */
	void BulletReload();
	
protected:
	/*
	 *	銃データの取得
	 */
	inline JSON GetData() { return json; }
};

#endif // !_WEAPONBASE_H_
