/*
 *  @file   SettingsManager.cpp
 *  @author Seki
 */

#include "SettingsManager.h"
#include "../../../Data/SaveData.h"
#include "../../Audio/AudioUtility.h"

/*
 *  @brief  初期化処理
 */
void SettingsManager::Initialize() {
    Load();
    AudioUtility::SetMasterVolume(masterVolume);
    AudioUtility::SetBGMVolume(bgmVolume);
    AudioUtility::SetSEVolume(seVolume);
}
/*
 *  @brief      セーブ処理
 *  @return     bool
 */
bool SettingsManager::Save() const {
    const std::string path = _SETTING_DATA_PATH;

    std::ofstream file(path);
    if (!file.is_open()) {
        assert(false && "Settings.json を開けませんでした");
        return false;
    }

    Orderd_JSON root;
    root["Version"] = 1;

    Orderd_JSON settings;
    settings["mouseSensitivity"] = mouseSensitivity;
    settings["masterVolume"] = masterVolume;
    settings["bgmVolume"] = bgmVolume;
    settings["seVolume"] = seVolume;

    root["Settings"] = settings;

    file << root.dump(4);
    return !file.fail();
}
/*
 *  @brief      ロード処理
 *  @return     bool
 */
bool SettingsManager::Load() {
    const std::string path = _SETTING_DATA_PATH;

    // 初回起動はデフォルト生成
    if (!std::filesystem::exists(path)) {
        mouseSensitivity = 1.0f;
        masterVolume = 1.0f;
        bgmVolume = 1.0f;
        seVolume = 1.0f;
        Save();
        return true;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        assert(false && "Settings.json を開けませんでした");
        return false;
    }

    JSON root;
    file >> root;

    const JSON& settings = root["Settings"];

    mouseSensitivity = settings.value("mouseSensitivity", 1.0f);
    masterVolume = settings.value("masterVolume", 1.0f);
    bgmVolume = settings.value("bgmVolume", 1.0f);
    seVolume = settings.value("seVolume", 1.0f);

    return true;
}
/*
 *  @brief      SettingData -> JSONへ変換
 *  @param[in]  const SettingsData& data
 *  @return     Orderd_JSON
 */
Orderd_JSON SettingsManager::ToJSON(const SettingsData& data) {
    Orderd_JSON json;
    json["Version"] = 1;

    json["Input"]["MouseSensitivity"] = data.mouseSensitivity;

    json["Audio"]["Master"] = data.masterVolume;
    json["Audio"]["BGM"] = data.bgmVolume;
    json["Audio"]["SE"] = data.seVolume;

    return json;
}
/*
 *  @brief      セーブ用設定データの収集
 *  @return     SettingsData
 */
SettingsData SettingsManager::GetSaveData() const {
    SettingsData data{};
    data.mouseSensitivity = mouseSensitivity;
    data.masterVolume = masterVolume;
    data.bgmVolume = bgmVolume;
    data.seVolume = seVolume;
    return data;
}
/*
 *  @brief      セーブデータから各種数値を設定
 *  @param[in]  const SettingsData& data
 */
void SettingsManager::ApplyLoadData(const SettingsData& data) {
    mouseSensitivity = data.mouseSensitivity;
    masterVolume = data.masterVolume;
    bgmVolume = data.bgmVolume;
    seVolume = data.seVolume;
}