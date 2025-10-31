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

/*
 *	音源管理クラス
 */
class AudioManager {
private:
	std::unique_ptr<BGMManager> bgmManager;	// BGMManager(常に持っている想定)
	std::unique_ptr<SEManager> seManager;	// SEManager (常に持っている想定)

	int bgmVolume = 0;						// BGM音量	 (セーブ項目)
	int seVolume = 0;						// SE音量	 (セーブ項目)

public:
	/*
	 *	コンストラクタ
	 */
	AudioManager() : bgmManager(std::make_unique<BGMManager>()), seManager(std::make_unique<SEManager>()) {}
	/*
	 *	デストラクタ
	 */
	~AudioManager() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	ロード済みのデータの設定
	 */
	void Setup();
	/*
	 *	更新処理
	 */
	void Update();

public:
	/*
	 *	BGMの再生処理
	 */
	void PlayBGM();
	/*
	 *	BGMの停止処理
	 */
	void StopBGM();
	/*
	 *	BGMの変更処理
	 *  @param[in]	const std::string& setKeyName	変更するBGM名
	 */
	void ChangeBGM(const std::string& setKeyName);

public:
	/*
	 *	BGM音量の取得
	 *  @return		int
	 */
	inline int GetBGMVolume() const { return bgmVolume; }
	/*
	 *	BGM音量の設定
	 *  @param[in]	const int& setVolume	音量
	 */
	inline void SetBGMVolume(const int& setVolume) { bgmVolume = setVolume; }
};
#endif // !_AUDIO_MANAGER_H_
