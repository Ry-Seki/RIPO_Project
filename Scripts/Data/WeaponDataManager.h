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
	static constexpr const char* WEAPON_DATA_PATH = "Data/Player/Weapon/WeaponData.json";

public:
	WeaponDataManager() = default;
	~WeaponDataManager() = default;

private:
	/*
	 *	jsonデータをマップに登録
	 */
	void SetMapJsonData(const JSON& setData) {
		auto damage = static_cast<char>(GameEnum::WeaponData::DefaultDamage);
		auto magazine = static_cast<char>(GameEnum::WeaponData::MagazineCapacity);
		auto coolTime = static_cast<char>(GameEnum::WeaponData::ShotCoolTime);
		auto reloading = static_cast<char>(GameEnum::WeaponData::ReloadingTime);

		for (int i = 0; i < static_cast<int>(GameEnum::Weapon::Max); i++) {
			weaponsData[i].defaultDamage = setData[static_cast<char>(i)][damage].get<float>();
			weaponsData[i].magazineCapacity = setData[static_cast<char>(i)][magazine].get<int>();
			weaponsData[i].shotCoolTime = setData[static_cast<char>(i)][coolTime].get<float>();
			weaponsData[i].reloadingTime = setData[static_cast<char>(i)][reloading].get<float>();
		}
	}

public:
	/*
	 *	初期化
	 */
	void Initialize() {
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
