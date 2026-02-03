/*
 *  @file   SettingsManager.cpp
 *  @author Seki
 */

#include "SettingsManager.h"
#include "../../../Data/SaveData.h"

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