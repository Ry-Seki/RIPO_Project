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
#include "../../../Data/WeaponDataManager.h"
#include <string>

 // 別名定義
class WeaponBase;
using WeaponBasePtr = std::shared_ptr<WeaponBase>;
/*
 *	武器のウデの基底クラス
 */
class WeaponBase : public ArmBase {
public:
	GameEnum::Weapon number;		// 番号
	int ammoCount;					// 残弾数
	int ammoCountMax;				// 弾の最大数
	float reloadingTime;			// リロードの残り時間
	float reloadingTimeMax;			// リロードに掛かる時間
	float shotCoolTime;				// 射撃のクールタイム
	float shotCoolTimeMax;			// 射撃のクールタイムの最大
	bool reload;					// リロードフラグ

	const std::string BULLET_NAME;	// 弾の名前
	const Vector3 BULLET_AABB_MIN;	// 弾のAABBMin
	const Vector3 BULLET_AABB_MAX;	// 弾のAABBMax

public:
	WeaponBase();
	virtual ~WeaponBase() = default;

public:
	/*
	 *	初期化処理
	 */
	virtual void Initialize() override;
	/*
	 *	更新処理
	 */
	virtual void ArmUpdate(float deltaTime, ActionMapBase::ActionState action) override;
		 
protected:
	/*
	 *	銃を撃つ処理
	 */
	void ShotBullet();

	/*
	 *	リロード
	 */
	void BulletReload(float deltaTime);

public:
	/*
	 *	番号取得
	 */
	GameEnum::Weapon GetNumber();
};

#endif // !_WEAPONBASE_H_
