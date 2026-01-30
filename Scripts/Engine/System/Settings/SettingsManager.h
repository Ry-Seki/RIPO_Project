/*
 *	@file	SettingsManager.h
 *	@author	Seki
 */

#ifndef _SETTINGS_MANAGER_H_
#define _SETTINGS_MANAGER_H_

#include "../../Singleton.h"

#include <algorithm>

// 前方宣言
struct SettingsData;

/*
 *	@brief	設定を管理するクラス
 */
class SettingsManager : public Singleton<SettingsManager> {
	friend class Singleton<SettingsManager>;

private:
    float mouseSensitivity = 0.0f;
    float masterVolume = 0.0f;
    float bgmVolume = 0.0f;
    float seVolume = 0.0f;

public:
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
     *  @return     float(0.0f～1.0f)
     */
    inline float GetMasterVolume() const{ return masterVolume; }
    /*
     *  @brief      マスター音量の設定
     *  @param[in]  float setValue(0.0f～1.0f)
     */
    inline void SetMasterVolume(float setValue) {
        masterVolume = std::clamp(setValue, 0.0f, 1.0f);
    }
    /*
     *  @brief      マスター音量の上昇
     *  @param[in]  float setValue
     */
    inline void AddMasterVolume(float setValue) {
        SetMasterVolume(masterVolume + setValue);
    }
    /*
     *  @brief      マスター音量の低下
     *  @param[in]  float setValue
     */
    inline void SubMasterVolue(float setValue) {
        SetMasterVolume(masterVolume - setValue);
    }
    /*
     *  @brief      BGM音量の取得
     *  @return     float(0.0f～1.0f)
     */
    inline float GetBGMVolume() const { return bgmVolume; }
    /*
     *  @brief      BGM音量の設定
     *  @param[in]  float setValue(0.0f～1.0f)
     */
    inline void SetBGMVolume(float setValue) {
        bgmVolume = std::clamp(setValue, 0.0f, 1.0f);
    }
    /*
     *  @brief      BGM音量の上昇
     *  @param[in]  flaot setValue
     */
    inline void AddBGMVolume(float setValue) {
        SetBGMVolume(bgmVolume + setValue);
    }
    /*
     *  @brief      BGM音量の低下
     *  @param[in]  float setValue
     */
    inline void SubBGMVolume(float setValue) {
        SetBGMVolume(bgmVolume - setValue);
    }
    /*
     *  @brief      SE音量の取得
     *  @return     float(0.0f～1.0f)
     */
    inline float GetSEVolume() const{ return seVolume; }
    /*
     *  @brief      SE音量の設定
     *  @param[in]  float setVolume(0.0f～1.0f)
     */
    inline void SetSEVolume(float setValue) {
        seVolume = std::clamp(setValue, 0.0f, 1.0f);
    }
    /*
     *  @brief      SE音量の上昇
     *  @param[in]  float setValue
     */
    inline void AddSEVolume(float setValue) {
        SetSEVolume(seVolume + setValue);
    }
    /*
     *  @brief      SE音量の低下
     *  @param[in]  float setValue
     */
    inline void SubSEVolume(float setValue) {
        SetSEVolume(seVolume - setValue);
    }
};

#endif // !_SETTINGS_MANAGER_H_