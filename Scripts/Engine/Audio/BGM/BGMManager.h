/*
 *	@file	BGMManager.h
 *	@author	Seki
 */

#ifndef _BGM_MANAGER_H_
#define _BGM_MANAGER_H_

#include "../AudioSource.h"
#include "../BGM/BGMSource.h"

#include <DxLib.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
/*
 *	BGMを管理するクラス
 */
class BGMManager {
private:
	std::unordered_map<std::string, int> bgmHandleMap;	// BGMハンドルの連想配列
	AudioSourcePtr currentSource;						// 現在のBGM
	mutable std::mutex mtx;                             // スレッド安全用

public:
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	ロード済みのデータの設定(コールバック)
	 *  @param[in]	
	 */
	void Setup();
	/*
	 *	更新処理
	 */
	void Update();
	/*
	 *	描画処理
	 */
	void Render();
	/*
	 *	BGMの破棄処理
	 */
	void Teardown();

public:
	/*
	 *	BGMの再生処理
	 *  @param[in]	int setVolume	音量
	 */
	void PlayBGM(int setVolume);
	/*
	 *	BGMの停止処理
	 */
	void StopBGM();
	/*
	 *	BGMの変更処理
	 *  @param[in]	consst std::string	setBGMNama	変更先のBGM名
	 */
	void ChangeBGM(const std::string setBGMNama);

public:
	/*
	 *	BGMハンドルの登録
	 *	@param[in]	std::string	setKeyName	 登録するBGM名
	 *	@param[in]	int setHandle			 登録する音源ハンドル
	 */
	void RegisterBGMHandle(std::string setKeyName, int setHandle);
	/*
	 *	BGMハンドルが登録済みか判定
	 *  @param[in]	std::string setKeyName	 調べるBGM名
	 *  @return		bool
	 */
	bool ExistBGMHandle(std::string setKeyName);

public:
	/*
	 *	現在のBGMの取得
	 *	@return		AudioSourcePtr
	 */
	inline AudioSourcePtr GetCurrentSource() const { return currentSource; }

};

#endif // !_BGM_MANAGER_H_
