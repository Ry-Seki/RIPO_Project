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
    AudioUtility::SetMasterVolume(masterVolume * _DEVIDE_MAX_VOLUME);
    AudioUtility::SetBGMVolume(bgmVolume * _DEVIDE_MAX_VOLUME);
    AudioUtility::SetSEVolume(seVolume * _DEVIDE_MAX_VOLUME);
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
        mouseSensitivity = _MAX_VOLUME;
        masterVolume = _MAX_VOLUME;
        bgmVolume = _MAX_VOLUME;
        seVolume = _MAX_VOLUME;
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

    mouseSensitivity = settings.value("mouseSensitivity", _MAX_VOLUME);
    masterVolume = settings.value("masterVolume", _MAX_VOLUME);
    bgmVolume = settings.value("bgmVolume", _MAX_VOLUME);
    seVolume = settings.value("seVolume", _MAX_VOLUME);

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