/*
 *	@file	SettingsManager.h
 *	@author	Seki
 */

#ifndef _SETTINGS_MANAGER_H_
#define _SETTINGS_MANAGER_H_

#include "../../Singleton.h"
#include "../../JSON.h"
#include "../../Audio/AudioUtility.h"

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cassert>

// 前方宣言
struct SettingsData;

/*
 *	@brief	設定を管理するクラス
 */
class SettingsManager : public Singleton<SettingsManager> {
	friend class Singleton<SettingsManager>;

private:
    int mouseSensitivity = 0;
    int masterVolume = 0;
    int bgmVolume = 0;
    int seVolume = 0;

    static constexpr const char* _SETTING_DATA_PATH = "SaveData/Settings.json";
    static constexpr const int _MAX_VOLUME = 10;
    static constexpr float _DEVIDE_MAX_VOLUME = 1.0f / 10.0f;

public:
    /*
     *  @brief  初期化処理
     */
    void Initialize();
    /*
     *  @brief      セーブ処理
     *  @return     bool
     */
    bool Save() const;
    /*
     *  @brief      ロード処理
     *  @return     bool
     */
    bool Load();

public:
    /*
     *  @brief      SettingData -> JSONへ変換
     *  @param[in]  const SettingsData& data
     *  @return     Orderd_JSON
     */
    Orderd_JSON ToJSON(const SettingsData& data);
    /*
     *  @brief      セーブ用設定データの収集
     *  @return     SettingsData
     */
    SettingsData GetSaveData() const;
    /*
     *  @brief      セーブデータから各種数値を設定
     *  @param[in]  const SettingsData& data
     */
    void ApplyLoadData(const SettingsData& data);

public:
    /*
     *  @brief      マウス感度の取得
     *  @return     float(0.0f～1.0f)
     */
    inline float GetMouseSensitivity() const { return mouseSensitivity; }
    /*
     *  @brief      マウス感度の設定
     *  @param[in]  float setValue(0.01f～1.0f)
     */
    inline void SetMouseSensitivity(float setValue) {
        mouseSensitivity = std::clamp(setValue, 0.01f, 1.0f);
    }
    /*
     *  @brief      マウス感度の上昇
     *  @param[in]  float setValue
     *  TODO : 最低値から上昇した時に0.01分残るのを対策する
     */
    inline void AddMouseSensitivity(float setValue) {
        SetMouseSensitivity(mouseSensitivity + setValue);
    }
    /*
     *  @brief      マウス感度の低下
     *  @param[in]  float setValue
     */
    inline void SubMouseSensitivity(float setValue) {
        SetMouseSensitivity(mouseSensitivity - setValue);
    }
    /*
     *  @brief      マスター音量の取得
     *  @return     int(10段階)
     */
    inline int GetMasterVolume() const{ return masterVolume; }
    /*
     *  @brief      マスター音量の設定
     *  @param[in]  int setValue(0～10)
     */
    inline void SetMasterVolume(int setValue) {
        masterVolume = std::clamp(setValue, 0, _MAX_VOLUME);
        AudioUtility::SetMasterVolume(masterVolume * _DEVIDE_MAX_VOLUME);
    }
    /*
     *  @brief      マスター音量の上昇
     *  @param[in]  int setValue
     */
    inline void AddMasterVolume(int setValue) {
        SetMasterVolume(masterVolume + setValue);
    }
    /*
     *  @brief      マスター音量の低下
     *  @param[in]  int setValue
     */
    inline void SubMasterVolume(int setValue) {
        SetMasterVolume(masterVolume - setValue);
    }
    /*
     *  @brief      BGM音量の取得
     *  @return     int(10段階)
     */
    inline int GetBGMVolume() const { return bgmVolume; }
    /*
     *  @brief      BGM音量の設定
     *  @param[in]  int setValue(0～10)
     */
    inline void SetBGMVolume(int setValue) {
        bgmVolume = std::clamp(setValue, 0, _MAX_VOLUME);
        AudioUtility::SetBGMVolume(bgmVolume * _DEVIDE_MAX_VOLUME);
    }
    /*
     *  @brief      BGM音量の上昇
     *  @param[in]  int setValue
     */
    inline void AddBGMVolume(int setValue) {
        SetBGMVolume(bgmVolume + setValue);
    }
    /*
     *  @brief      BGM音量の低下
     *  @param[in]  int setValue
     */
    inline void SubBGMVolume(int setValue) {
        SetBGMVolume(bgmVolume - setValue);
    }
    /*
     *  @brief      SE音量の取得
     *  @return     int(0～10)
     */
    inline int GetSEVolume() const{ return seVolume; }
    /*
     *  @brief      SE音量の設定
     *  @param[in]  int setVolume(0～10)
     */
    inline void SetSEVolume(int setValue) {
        seVolume = std::clamp(setValue, 0, _MAX_VOLUME);
        AudioUtility::SetSEVolume(seVolume * _DEVIDE_MAX_VOLUME);
    }
    /*
     *  @brief      SE音量の上昇
     *  @param[in]  int setValue
     */
    inline void AddSEVolume(int setValue) {
        SetSEVolume(seVolume + setValue);
    }
    /*
     *  @brief      SE音量の低下
     *  @param[in]  int setValue
     */
    inline void SubSEVolume(int setValue) {
        SetSEVolume(seVolume - setValue);
    }
};

#endif // !_SETTINGS_MANAGER_H_