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
	void Initialize() override;
	/*
	 *	更新処理
	 */
	void ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) override;
		 
	/*
	 *	各武器の最初のUpdateの直前に呼び出される処理
	 */
	virtual void WeaponStart() {}
	/*
	 *	各武器の初期化処理
	 */
	virtual void WeaponInitialize() {}
	/*
	 *	各武器の更新処理
	 */
	virtual void WeaponUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {}

protected:
	/*
	 *	銃を撃つ処理
	 */
	void ShotBullet(Engine* engine);

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
