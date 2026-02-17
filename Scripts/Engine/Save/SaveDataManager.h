/*
 *	@file	SaveDataManager.h
 *	@author	Seki
 */

#ifndef _SAVE_DATA_MANAGER_H_
#define _SAVE_DATA_MANAGER_H_

#include "../Singleton.h"
#include "../JSON.h"
#include "../../Data/SaveData.h"
#include "../GameConst.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cassert>

// 前方宣言
struct ActionContext;

/*
 *	@brief	セーブデータの管理クラス
 */
class SaveDataManager : public Singleton<SaveDataManager> {
	friend class Singleton<SaveDataManager>;

private:
	int currentSlotIndex = -1;							// 現在のスロット番号
	std::string currentSlotPath;							// 現在のスロット
	SaveData currentSaveData;							// 現在プレイ中のスロット
	SaveData autoSaveData;								// オートセーブ専用

	const std::string _SAVE_FILE_PATH = "SaveData/";	// ファイルパス
	const std::string _JSON_PATH = ".json";				// JSON拡張子
	const std::string _AUTO_SAVE = "AutoSave";			// オートセーブ

	const int _SAVE_VERSION = 1;						// 将来のバージョン管理用

private:
	/*
	 *	@brief	コンストラクタ
	 */
	SaveDataManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SaveDataManager() override {}

private:
	/*
 *	@brief		セーブ処理
 *  @param[in]	const SaveData& data
 *  @param[in]	const std::string& slotPath
 *  @return		bool
 */
	bool Save(const SaveData& data, const std::string& slotPath);
	/*
	 *	@brief		ロード処理
	 *  @param[out]	SaveData& outData
	 *  @param[in]	const std::string& slotPath
	 *  @return		bool
	 */
	bool Load(SaveData& outData, const std::string& slotPath);
	/*
	 *	@brief		SaveData->JSONへ変換
	 *	@param[in]	const SaveData& data
	 *	@return		JSON
	 */
	Orderd_JSON ToJSON(const SaveData& data);
	/*
	 *	@brief		JSON->SaveDataへ変換
	 *	@param[in]	const JSON& json
	 *	@return		SaveData
	 */
	SaveData SaveDataFromJSON(const JSON& json);
	/*
	 *	@brief		ゲーム進行データ->JSONへ変換
	 *	@param[in]	const GameProgressData& data
	 *  @return		JSON
	 */
	Orderd_JSON ToJSON(const GameProgressData& data);
	/*
	 *	@brief		JSON->ゲーム進行データへ変換
	 *	@param[in]	const JSON& json
	 *	@return		GameProgressData
	 */
	GameProgressData GameDataFromJSON(const JSON& json);
	/*
	 *	@brief		プレイヤーステータスレベルデータ->JSONへ変換
	 *	@param[in]	const PlayerProgressData& data
	 *  @return		JSON
	 */
	Orderd_JSON ToJSON(const PlayerStatusLevelData& data);
	/*
	 *	@brief		JSON->プレイヤーステータスレベルデータへ変換
	 *  @param[in]	const JSON& json
	 *  @return		PlayerStatusLevelData
	 */
	PlayerStatusLevelData PlayerDataFromJSON(const JSON& json);
	/*
	 *	@brief		ワールド進行データ->JSONへ変換
	 *	@param[in]	const WorldProgressData& data
	 *  @return		JSON
	 */
	Orderd_JSON ToJSON(const WorldProgressData& data);
	/*
	 *	@brief		JSON->ワールド進行データへ変換
	 *	@param[in]	const JSON& json
	 *	@return		WorldProgressData
	 */
	WorldProgressData WorldDataFromJSON(const JSON& json);
	/*
	 *	@brief		ダンジョン進行データ->JSONへ変換
	 *	@param[in]	const DungeonProgressData& data
	 *	@return		JSON
	 */
	Orderd_JSON ToJSON(const DungeonProgressData& data);
	/*
	 *	@brief		JSON->ダンジョン進行データへ変換
	 *	@param[in]	const JSON& json
	 *  @param[in]	int dungeonID
	 *	@return		DungeonProgressData
	 */
	DungeonProgressData DungeonDataFromJSON(const JSON& json, int dungeonID);

public:
	/*
	 *	@brief		初期化処理
	 */
	void Initialize();
	/*
	 *	@brief		選択されたスロットにセーブ
	 *  @return		bool
	 */
	bool SaveCurrentSlot();
	/*
	 *	@brief		選択されたスロットにロード
	 *	@return		bool
	 */
	bool LoadCurrentSlot();
	/*
	 *	@brief		オートセーブスロットにセーブ
	 *	@return		bool
	 */
	bool AutoSave();
	/*
	 *	@brief		オートセーブスロットからロード
	 *	@return		bool
	 */
	bool AutoSaveLoad();
	/*
	 *	@brief		スロット選択
	 *	@param[in]	int selectSlot
	 */
	void SelectSlot(int selectSlot);
	/*
	 *	@brief		そのデータが存在している(使用済み)か判定
	 *  @param[in]	int selectSlot
	 *	@return		bool
	 */
	bool Exists(int selectSlot);
	/*
	 *	@brief		セーブに必要なデータを集める
	 *	@param[in]	const ActionContext& context
	 */
	void CollectSaveData(const ActionContext& context);
	/*
	 *	@brief		セーブデータからデータを渡す
	 *	@param[in]	ActionContext& context
	 */
	void ApplyLoadData(ActionContext& context);
	/*
	 *	@brief		全てのセーブスロットの使用状態の確認
	 *	@return		std::vector<bool>
	 */
	std::vector<bool> GetAllSlotIsUsed();
	/*
	 *	@brief      指定スロットのGameProgressDataを取得
	 *	@return     std::vector<SaveData>
	 */
	std::vector<SaveData> GetAllSlotData();
	/*
	 *	@brief	クリア済みセーブデータのリセット
	 */
	void ResetClearSaveData();

public:
	/*
	 *	@brief		スロットの取得
	 *	@return		int
	 */
	inline int GetCurrentSlot() const { return currentSlotIndex; }
	/*
	 *	@brief		ファイルパスを生成
	 *	@param[in]	const std::string& slotPath
	 *	@return		std::string
	 */
	inline std::string MakeFilePath(const std::string& slotPath) { return _SAVE_FILE_PATH + slotPath + _JSON_PATH; }
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