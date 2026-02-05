/*
 *	@file	AudioManager.h
 *	@author	Seki
 */

#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "../Singleton.h"
#include "BGM/BGMManager.h"
#include "SE/SEManager.h"

#include <memory>
#include <algorithm>

/*
 *	@brief	音源管理クラス
 */
class AudioManager : public Singleton<AudioManager>{
	friend class Singleton<AudioManager>;

private:
	std::unique_ptr<BGMManager> bgmManager;	// BGMManager(常に持っている想定)
	std::unique_ptr<SEManager> seManager;	// SEManager (常に持っている想定)

	float masterVolume = 0.0f;					// マスター音量
	float bgmVolume = 0.0f;						// BGM音量
	float seVolume = 0.0f;						// SE音量

private:
	/*
	 *	@brief	コンストラクタ
	 */
	AudioManager() : bgmManager(std::make_unique<BGMManager>()), seManager(std::make_unique<SEManager>()) {}
	/*
	 *	@brief	デストラクタ
	 */
	~AudioManager() override = default;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief	更新処理
	 */
	void Update();

public:
	/*
	 *	@brief	BGMの再生処理
	 */
	void PlayBGM();
	/*
	 *	@brief	BGMの停止処理
	 */
	void StopBGM();
	/*
	 *	@brief		BGMの変更処理
	 *  @param[in]	const std::string& setKeyName	変更するBGM名
	 */
	void ChangeBGM(const std::string& setKeyName);
	/*
	 *	@brief		BGMハンドルの登録
	 *	@param[in]	const std::string&	setKeyName	登録するBGM名
	 *	@param[in]	const int setHandle				登録する音源ハンドル
	 */
	void RegisterBGMHandle(const std::string& setKeyName, const int setHandle);
	/*
	 *	@brief		BGM音量変更の適応
	 */
	void ApplyBGMVolume();

public:
	/*
	 *	@brief		SE再生処理
	 *  @param[in]	const std::string& setKeyName	再生するSE名
	 */
	void PlaySE(const std::string& setKeyName);
	/*
	 *	@brief		SEハンドルの登録
	 *	@param[in]	const std::string&	setKeyName	登録するSE名
	 *	@param[in]	const int setHandle				登録する音源ハンドル
	 */
	void RegisterSEHandle(const std::string& setKeyName, const int setHandle);

public:
	/*
	 *	@brief		マスター音量の取得
	 *  @return		float (0.0f～1.0f)
	 */
	inline float GetMasterVolume() const { return masterVolume; }
	/*
	 *	@brief		マスター音量の変更
	 *  @param[in]	float setVolume		音量 (0.0f～1.0f)
	 */
	inline void SetMasterVolume(float setVolume) {
		masterVolume = std::clamp(setVolume, 0.0f, 1.0f);
		ApplyBGMVolume();
	}
	/*
	 *	@brief		BGM音量の取得
	 *  @return		float (0.0f～1.0f)
	 */
	inline float GetBGMVolume() const { return bgmVolume; }
	/*
	 *	@brief		BGM音量の設定
	 *  @param[in]	float setVolume		音量 (0.0f～1.0f)
	 */
	inline void SetBGMVolume(float setVolume) { 
		bgmVolume = std::clamp(setVolume, 0.0f, 1.0f); 
		ApplyBGMVolume();
	}
	/*
	 *	@brief		SE音量の取得
	 *  @return		float (0.0f～1.0f)
	 */
	inline float GetSEVolume() const { return seVolume; }
	/*
	 *	@brief		SE音量の設定
	 *	@param[in]	float setVolume		音量 (0.0f～1.0f)
	 */
	inline void SetSEVolume(float setVolume) {
		seVolume = std::clamp(setVolume, 0.0f, 1.0f);
	}
};
#endif // !_AUDIO_MANAGER_H_
