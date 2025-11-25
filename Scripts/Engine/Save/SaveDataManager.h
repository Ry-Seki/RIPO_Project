/*
 *	@file	SaveDataManager.h
 */

#ifndef _SAVE_DATA_MANAGER_H_
#define _SAVE_DATA_MANAGER_H_

#include "../Singleton.h"
#include "../JSON.h"
#include "../../Data/SaveData.h"

#include <string>
#include <fstream>
#include <filesystem>

class SaveDataManager : public Singleton<SaveDataManager> {
	friend class Singleton<SaveDataManager>;

private:
	SaveData currentSaveData;						// 現在プレイ中のスロット
	SaveData autoSaveData;							// オートセーブ専用
	std::string currentSlot;						// 現在のスロット

	const std::string _SAVE_PATH = "SaveData/";		// ファイルパス
	const int _SAVE_VERSION = 1;					// 将来のバージョン管理用

private:
	/*
	 *	@brief	コンストラクタ
	 */
	SaveDataManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SaveDataManager() override {}

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() {
		if (!std::filesystem::exists(_SAVE_PATH)) {
			std::filesystem::create_directories(_SAVE_PATH);
		}
	}
	/*
	 *	@brief		スロットの選択
	 *	@param[in]	int setSlot
	 */
	void SelectSlot(int setSlot) {
		if (setSlot >= 1 && setSlot <= 3) {
			currentSlot = "Slot" + std::to_string(setSlot);
		}
		else {
			currentSlot = "AutoSave";
		}
	}
	/*
	 *	@brief		JSON->SaveDataへの変換
	 */
	SaveData FromJSONToSaveData(const JSON& json) {
		SaveData data;
		data.elapsedDay = json.value("elapsedDay", 0);
		data.isHalf = json.value("isHalf", false);
		data.playTime = json.value("playTime", 0);
		data.currentMoney = json.value("currentMoney", 0);
		data.dungeonFlags = json.value("dungeonFlags", 0);
		return data;
	}
	/*
	 *	@brief		SaveData->JSONへの変換
	 */
	JSON FromSaveDataToJSON(const SaveData& data) {
		JSON json;
		json["version"] = _SAVE_VERSION; // 将来の互換用
		json["elapsedDay"] = data.elapsedDay;
		json["isHalf"] = data.isHalf;
		json["playTime"] = data.playTime;
		json["currentMoney"] = data.currentMoney;
		json["dungeonFlags"] = data.dungeonFlags;
		return json;
	}
	/*
	 *	@brief		現在のセーブデータの読み込み
	 */
	bool LoadCurrentSlot() {
		return LoadSaveData(currentSaveData, currentSlot);
	}
	/*
	 *	@brief		セーブデータ読み込み
	 *  @param[in]	SaveData& saveData
	 *  @param[in]	const std::string& slotName
	 */
	bool LoadSaveData(SaveData& saveData, const std::string& slotName){
		std::string filePath = _SAVE_PATH + currentSlot + ".json";
		if (!std::filesystem::exists(filePath)) return false;

		std::ifstream in(filePath);
		if (!in.is_open()) return false;

		JSON json;
		in >> json;
		in.close();

		saveData = FromJSONToSaveData(json);
		return true;
	}
	/*
	 *	@brief		現在のセーブデータの書き込み
	 */
	bool Save() {
		std::string filePath = _SAVE_PATH + currentSlot + ".json";

		std::ofstream out(filePath);
		if (!out.is_open()) return false;

		JSON json = FromSaveDataToJSON(currentSaveData);

		out << json.dump(4);
		out.close();
		return true;
	}

	/*
	 *	@brief		オートセーブ書き込み
	 */
	bool SaveAuto() {
		std::string filePath = _SAVE_PATH + "AutoSave.json";

		std::ofstream out(filePath);
		if (!out.is_open()) return false;

		autoSaveData = currentSaveData;

		JSON json = FromSaveDataToJSON(autoSaveData);
		out << json.dump(4);
		out.close();
		return true;
	}

	/*
	 *	@brief		新規スロット作成
	 *	@param[in]	int setSlot
	 */
	void CreateInitData(int setSlot) {
		SelectSlot(setSlot);

		currentSaveData.elapsedDay = 0;
		currentSaveData.isHalf = false;
		currentSaveData.playTime = 0;
		currentSaveData.currentMoney = 0;
		currentSaveData.dungeonFlags = 0;

		Save();
	}

public:
	/*
	 *	@brief		セーブデータの取得
	 *	@return		SaveData&
	 */
	inline SaveData& GetSaveData() { return currentSaveData; }
	/*
	 *	@brief		セーブデータの取得
	 *	@return		const SaveData&
	 */
	inline const SaveData& GetSaveData() const { return currentSaveData; }

	/*
	 *	@brief		オートセーブデータの取得
	 *	@return		SaveData&
	 */
	inline SaveData& GetAutoSaveData() { return autoSaveData; }
	/*
	 *	@brief		オートセーブデータの取得
	 *	@return		const SaveData&
	 */
	inline const SaveData& GetAutoSaveData() const { return autoSaveData; }
};

#endif // !_SAVE_DATA_MANAGER_H_