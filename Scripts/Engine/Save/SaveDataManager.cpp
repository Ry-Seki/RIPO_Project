/*
 *	@file	SaveDataManager.cpp
 *	@author	Seki
 */

#include "SaveDataManager.h"
#include "../../Data/Dungeon/DungeonProgressData.h"
#include "../System/Status/PlayerStatusManager.h"
#include "../System/Money/MoneyManager.h"
#include "../System/World/WorldProgressManager.h"
#include "../Scene/GameState/ActionContext.h"
#include "../Manager/WeaponManager.h"

/*
 *	@brief	初期化処理
 */
void SaveDataManager::Initialize() {
    if (!std::filesystem::exists(_SAVE_FILE_PATH)) {
        std::filesystem::create_directories(_SAVE_FILE_PATH);
    }
    // 各スロットが存在しなければ初期データで作成
    const std::vector<std::string> slots = { "Slot1", "Slot2", "Slot3", _AUTO_SAVE };
    for (const auto& slot : slots) {
        const std::string path = MakeFilePath(slot);
        if (!std::filesystem::exists(path)) {
            SaveData defaultData{};
            Save(defaultData, slot);
        }
    }
    // 初期スロットはオートセーブにしておく
    SelectSlot(0);
    // currentSaveDataの初期化
    currentSaveData = SaveData{};
}
/*
 *	@brief		セーブ処理
 *  @param[in]	const SaveData& data
 *  @param[in]	const std::string& slotPath
 *  @return		bool
 */
bool SaveDataManager::Save(const SaveData& data, const std::string& slotPath) {
    const std::string path = MakeFilePath(slotPath);
    std::ofstream saveFile(path);
    if (!saveFile.is_open()) {
        assert(false && "セーブファイルを開けませんでした");
        return false;
    }
    Orderd_JSON json = ToJSON(data);
    // ファイルの書き込み
    saveFile << json.dump(4);
    if (saveFile.fail()) {
        assert(false && "セーブデータの書き込みに失敗しました");
        return false;
    }
    return true;
}
/*
 *	@brief		ロード処理
 *  @param[out]	SaveData& outData
 *  @param[in]	const std::string& slotPath
 *  @return		bool
 */
bool SaveDataManager::Load(SaveData& outData, const std::string& slotPath) {
    const std::string path = MakeFilePath(slotPath);
    if (!std::filesystem::exists(path)) {
        assert(false && "ファイルが存在しませんでした");
        return false;
    }
    std::ifstream loadFile(path);
    if (!loadFile.is_open()) {
        assert(false && "ファイルが開けませんでした");
        return false;
    }
    JSON json;
    try {
        loadFile >> json;
    } catch (...) {
        assert(false && "JSONのパースに失敗しました");
        return false;
    }
    outData = SaveDataFromJSON(json);
    return true;
}
/*
 *	@brief		SaveData->JSONへ変換
 *	@param[in]	const SaveData& data
 *	@return		JSON
 */
Orderd_JSON SaveDataManager::ToJSON(const SaveData& data) {
    Orderd_JSON json;
    json["Version"] = _SAVE_VERSION;
    json["IsUsed"] = data.isUsed;
    json["Game"] = ToJSON(data.game);
    json["Player"] = ToJSON(data.player);
    json["World"] = ToJSON(data.world);
    return json;
}
/*
 *	@brief		JSON->SaveDataへ変換
 *	@param[in]	const JSON& json
 *	@return		SaveData
 */
SaveData SaveDataManager::SaveDataFromJSON(const JSON& json) {
    SaveData data{};
    data.isUsed = json.value("IsUsed", false);
    data.game = GameDataFromJSON(json["Game"]);
    data.player = PlayerDataFromJSON(json["Player"]);
    data.world = WorldDataFromJSON(json["World"]);
    return data;
}
/*
 *	@brief		ゲーム進行データ->JSONへ変換
 *	@param[in]	const GameProgressData& data
 *  @return		JSON
 */
Orderd_JSON SaveDataManager::ToJSON(const GameProgressData& data) {
    Orderd_JSON json;
    json["playTime"] = data.playTime;
    json["elapsedDay"] = data.elapsedDay;
    json["isClear"] = data.isClear;
    json["clearCount"] = data.clearCount;
    json["isHalfDay"] = data.isHalfDay;
    json["isWeapon"] = data.isWeapon;
    json["currentMoney"] = data.currentMoney;
    json["totalTreasureCount"] = data.totalTreasureCount;
    return json;
}
/*
 *	@brief		JSON->ユーザー進行データ
 *	@param[in]	JSON json
 *	@return		GameProgressData
 */
GameProgressData SaveDataManager::GameDataFromJSON(const JSON& json) {
    GameProgressData data{};
    data.playTime = json.value("playTime", 0);
    data.elapsedDay = json.value("elapsedDay", 0);
    data.isClear = json.value("isClear", false);
    data.clearCount = json.value("ClearCount", 0);
    data.isHalfDay = json.value("isHalfDay", false);
    data.isWeapon = json.value("isWeapon", false);
    data.currentMoney = json.value("currentMoney", 0);
    data.totalTreasureCount = json.value("totalTreasureCount", 0);
    return data;
}
/*
 *	@brief		プレイヤーステータスレベルデータ->JSONへ変換
 *	@param[in]	const PlayerProgressData& data
 *  @return		JSON
 */
Orderd_JSON SaveDataManager::ToJSON(const PlayerStatusLevelData& data) {
    Orderd_JSON json;
    json["hpLevel"] = data.hpLevel;
    json["staminaLevel"] = data.staminaLevel;
    json["strengthLevel"] = data.strengthLevel;
    json["resistTimeLevel"] = data.resistTimeLevel;
    return json;
}
/*
 *	@brief		JSON->プレイヤーステータスレベルデータへ変換
 *  @param[in]	const JSON& json
 *  @return		PlayerStatusLevelData
 */
PlayerStatusLevelData SaveDataManager::PlayerDataFromJSON(const JSON& json) {
    PlayerStatusLevelData data{};
    data.hpLevel = json.value("hpLevel", 0);
    data.staminaLevel = json.value("staminaLevel", 0);
    data.strengthLevel = json.value("strengthLevel", 0);
    data.resistTimeLevel = json.value("resistTimeLevel", 0);
    return data;
}
/*
 *	@brief		ワールド進行データ->JSONへ変換
 *	@param[in]	const WorldProgressData& data
 *  @return		JSON
 */
Orderd_JSON SaveDataManager::ToJSON(const WorldProgressData& data) {
    Orderd_JSON json;
    json["getTreasureIDList"] = data.getTreasureIDList;
    // ダンジョン進行
    Orderd_JSON dungeonJSON = Orderd_JSON::object();
    for (const auto& [dungeonID, progress] : data.dungeonProgress) {
        dungeonJSON[std::to_string(dungeonID)] = ToJSON(progress);
    }
    json["dungeonProgress"] = dungeonJSON;

    return json;
}
/*
 *	@brief		JSON->ワールド進行データへ変換
 *	@param[in]	const JSON& json
 *	@return		WorldProgressData
 */
WorldProgressData SaveDataManager::WorldDataFromJSON(const JSON& json) {
    WorldProgressData data{};
    data.getTreasureIDList = json.value("getTreasureIDList", std::vector<int>{});
    // ダンジョン進行
    if (json.contains("dungeonProgress")) {
        for (auto& [key, value] : json["dungeonProgress"].items()) {
            int dungeonID = std::stoi(key);
            data.dungeonProgress[dungeonID] = DungeonDataFromJSON(value, dungeonID);
        }
    }
    return data;
}
/*
 *	@brief		ダンジョン進行データ->JSONへ変換
 *	@param[in]	const DungeonProgressData& data
 *	@return		JSON
 */
Orderd_JSON SaveDataManager::ToJSON(const DungeonProgressData& data) {
    Orderd_JSON json;
    json["isBossDefeated"] = data.isBossDefeated;
    // 通常お宝
    JSON treasureJSON;
    for (const auto& [treasureID, flag] : data.treasureFlagMap) {
        if (!flag) continue;

        treasureJSON[std::to_string(treasureID)] = true;
    }
    json["treasureFlags"] = treasureJSON;
    // イベントお宝
    JSON eventJSON;
    for (const auto& [eventID, flag] : data.eventTreasureFlagMap) {
        if (!flag) continue;

        eventJSON[std::to_string(eventID)] = true;
    }
    json["eventTreasureFlags"] = eventJSON;

    return json;
}
/*
 *	@brief		JSON->ダンジョン進行データへ変換
 *	@param[in]	const JSON& json
 *  @param[in]  int dungeonID
 *	@return		DungeonProgressData
 */
DungeonProgressData SaveDataManager::DungeonDataFromJSON(const JSON& json, int dungeonID) {
    DungeonProgressData data;
    data.dungeonID = dungeonID;
    data.isBossDefeated = json.value("isBossDefeated", false);
    // 通常お宝
    if (json.contains("treasureFlags")) {
        for (auto& [key, value] : json["treasureFlags"].items()) {
            int treasureID = std::stoi(key);
            bool obtained = value.get<bool>();
            if (obtained) {
                data.treasureFlagMap[treasureID] = true;
            }
        }
    }
    // イベントお宝
    if (json.contains("eventTreasureFlags")) {
        for (auto& [key, value] : json["eventTreasureFlags"].items()) {
            int eventID = std::stoi(key);
            bool flag = value.get<bool>();
            if (!flag) continue;

            data.eventTreasureFlagMap[eventID] = true;
        }
    }
    return data;
}
/*
 *	@brief		選択されたスロットにセーブ
 *  @return		bool
 */
bool SaveDataManager::SaveCurrentSlot() {
    currentSaveData.isUsed = true;
    return Save(currentSaveData, currentSlotPath);
}
/*
 *	@brief		オートセーブスロットにセーブ
 *	@return		bool
 */
bool SaveDataManager::AutoSave() {
    currentSaveData.isUsed = true;
    return Save(currentSaveData, _AUTO_SAVE);
}
/*
 *	@brief		選択されたスロットにロード
 *	@return		bool
 */
bool SaveDataManager::LoadCurrentSlot() {
    return Load(currentSaveData, currentSlotPath);
}
/*
 *	@brief		オートセーブスロットからロード
 *	@return		bool
 */
bool SaveDataManager::AutoSaveLoad() {
    return Load(currentSaveData, _AUTO_SAVE);
}
/*
 *	@brief		スロット選択
 *	@param[in]	int selectSlot
 */
void SaveDataManager::SelectSlot(int selectSlot) {
    if (selectSlot >= 1 && selectSlot <= 3) {
        currentSlotIndex = selectSlot;
        currentSlotPath = "Slot" + std::to_string(selectSlot);
    } else {
        currentSlotIndex = 0;
        currentSlotPath = _AUTO_SAVE;
    }
}
/*
 *	@brief		そのデータが存在している(使用済み)か判定
 *  @param[in]	int selectSlot
 *	@return		bool
 */
bool SaveDataManager::Exists(int selectSlot) {
    SaveData data{};
    std::string slot = "";
    // スロット番号が有効か判定
    if (selectSlot >= GameConst::SELECT_SAVE_SLOT_MIN && selectSlot <= GameConst:: SELECT_SAVE_SLOT_MAX) {
        slot = "Slot" + std::to_string(selectSlot);
    } else {
        slot = _AUTO_SAVE;
    }
    // ロード出来なかった場合、中止
    if (!Load(data, slot)) {
        assert(false && "セーブスロットがありませんでした");
        return false;
    }
    return data.isUsed;
}
/*
 *	@brief		セーブに必要なデータを集める
 *	@param[in]	const ActionContext& context
 */
void SaveDataManager::CollectSaveData(const ActionContext& context) {
    // World
    currentSaveData.world
        = WorldProgressManager::GetInstance().GetSaveData();

    // Game
    currentSaveData.game = context.GetSaveData();
    currentSaveData.game.currentMoney
        = MoneyManager::GetInstance().GetCurrentMoney();
    currentSaveData.game.totalTreasureCount =
        currentSaveData.world.getTreasureIDList.size();
    currentSaveData.game.isWeapon
        = WeaponManager::GetInstance().IsSubmachineGun();

    // Player
    currentSaveData.player
        = PlayerStatusManager::GetInstance().GetSaveData();


}
/*
 *	@brief		セーブデータからデータを渡す
 *	@param[in]	ActionContext& context
 */
void SaveDataManager::ApplyLoadData(ActionContext& context) {
    // Game
    context.ApplyLoadData(currentSaveData.game);
    MoneyManager::GetInstance().SetMoney(currentSaveData.game.currentMoney);
    WeaponManager::GetInstance().SetIsSubmachinGun(currentSaveData.game.isWeapon);
    // Player
    PlayerStatusManager::GetInstance().ApplyLoadData(currentSaveData.player);
    // World
    WorldProgressManager::GetInstance().SetWorldProgressData(currentSaveData.world);

}
/*
 *	@brief		全てのセーブスロットの使用状態の確認
 *	@return		std::vector<bool>
 */
std::vector<bool> SaveDataManager::GetAllSlotIsUsed() {
    std::vector<bool> result;
    for (int i = 0; i <= GameConst::SELECT_SAVE_SLOT_MAX; ++i) {
        result.push_back(Exists(i));
    }
    return result;
}
/*
 *	@brief      指定スロットのGameProgressDataを取得
 *	@return     std::vector<SaveData>
 */
std::vector<SaveData> SaveDataManager::GetAllSlotData() {
    std::vector<SaveData> result;
    
    // オートセーブ
    {
        SaveData data{};
        if (Load(data, _AUTO_SAVE)) {
            result.push_back(data);
        }
        else {
            result.push_back(SaveData{});
        }
    }
    // 通常スロット
    for (int i = GameConst::SELECT_SAVE_SLOT_MIN;
         i <= GameConst::SELECT_SAVE_SLOT_MAX; ++i) {
        SaveData data{};
        std::string slot = "Slot" + std::to_string(i);

        if (Load(data, slot)) {
            result.push_back(data);
        }
        else {
            result.push_back(SaveData{});
        }
    }
    return result;
}
/*
 *	@brief	クリア済みセーブデータのリセット
 */

void SaveDataManager::ResetClearSaveData() {
    SaveData data{};
    if (!Load(data, currentSlotPath)) return;

    if (!data.game.isClear) return;

    // 一部のデータ以外の初期化
    WorldProgressData worldData{};
    PlayerStatusLevelData statusData{};
    MoneyManager::GetInstance().ResetMoney();
    WeaponManager::GetInstance().SetIsSubmachinGun(false);
    PlayerStatusManager::GetInstance().ApplyLoadData(statusData);
    data.game.currentMoney = 0;
    data.game.elapsedDay = 0;
    data.game.isHalfDay = false;
    data.game.isWeapon = false;
    data.game.totalTreasureCount = 0;
    data.game.clearCount++;
    data.player.hpLevel = 0;
    data.player.staminaLevel = 0;
    data.player.strengthLevel = 0;
    data.player.resistTimeLevel = 0;

    data.world = WorldProgressData{};

    Save(data, currentSlotPath);
    currentSaveData = data;
}
