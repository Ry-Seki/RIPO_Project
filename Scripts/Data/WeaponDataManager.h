/*
 *	@file	WeaponData.h
 *	@author Riku
 */

#ifndef _WEAPON_DATA_H_
#define _WEAPON_DATA_H_

#include "../Engine/Singleton.h"
#include "../Engine/Load/JSON/LoadJSON.h"
#include "../Engine/GameEnum.h"
#include "../Engine/Load/LoadManager.h"
#include <unordered_map>
#include <string>

/*
 *	銃のデータ管理クラス
 */
class WeaponDataManager : public Singleton<WeaponDataManager>{
	friend class Singleton<WeaponDataManager>;

private:
	struct WeaponData {
		float defaultDamage;	// 基礎攻撃力
		int magazineCapacity;	// 装填弾数
		float shotCoolTime;		// 射撃のクールタイム
		float reloadingTime;	// リロードの長さ
	};
	// 全銃のデータ
	std::unordered_map<int, WeaponData> weaponsData;

	// jsonデータのファイルパス
	const std::string WEAPON_DATA_PATH = "Data/Player/Weapon/WeaponData.json";
	// json内の文字列
	const std::string REVOLVER = "Revolver";
	const std::string SMG = "SubmachineGun";
	const std::string DAMAGE = "DefaultDamage";
	const std::string MAGAZINE = "MagazineCapacity";
	const std::string COOL_TIME = "ShotCoolTime";
	const std::string RELOADING = "ReloadingTime";
	// enum指定用map
	std::unordered_map<GameEnum::Weapon, std::string> weaponsName;

public:
	WeaponDataManager() = default;
	~WeaponDataManager() = default;

private:
	/*
	 *	jsonデータをマップに登録
	 */
	void SetMapJsonData(const JSON& setData) {
		for (int i = 0; i < static_cast<int>(GameEnum::Weapon::Max); i++) {
			auto weaponName = weaponsName[static_cast<GameEnum::Weapon>(i)];

			weaponsData[i].defaultDamage = setData[weaponName][DAMAGE].get<float>();
			weaponsData[i].magazineCapacity = setData[weaponName][MAGAZINE].get<int>();
			weaponsData[i].shotCoolTime = setData[weaponName][COOL_TIME].get<float>();
			weaponsData[i].reloadingTime = setData[weaponName][RELOADING].get<float>();
		}
	}

public:
	/*
	 *	初期化
	 */
	void Initialize() {
		weaponsName[GameEnum::Weapon::Revolver] = REVOLVER;
		weaponsName[GameEnum::Weapon::SubmachineGun] = SMG;
		auto json = LoadManager::GetInstance().LoadResource<LoadJSON>(WEAPON_DATA_PATH);
		LoadManager::GetInstance().SetOnComplete([this, json]() {
			SetMapJsonData(json->GetData());
		});
	}

public:
	/*
	 *	各銃のデータ取得
	 */
	inline WeaponData GetWeaponData(GameEnum::Weapon weapon) { 
		return weaponsData[static_cast<int>(weapon)];
	}

};

#endif // !_WEAPON_DATA_H_
