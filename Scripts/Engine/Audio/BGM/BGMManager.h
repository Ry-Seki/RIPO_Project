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
	std::unique_ptr<BGMSource> currentSource;			// 現在のBGM (常に持っている想定) 
	mutable std::mutex mtx;                             // スレッド安全用

public:
	/*
	 *	コンストラクタ
	 */
	BGMManager() : currentSource(std::make_unique<BGMSource>()) {}
	/*
	 *	デストラクタ
	 */
	~BGMManager();
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
	 *	@param[in]	const std::string&	setKeyName	登録するBGM名
	 *	@param[in]	const int setHandle				登録する音源ハンドル
	 */
	void RegisterBGMHandle(const std::string& setKeyName, const int setHandle);
	/*
	 *	BGMハンドルが登録済みか判定
	 *  @param[in]	const std::string& setKeyName	調べるBGM名
	 *  @return		bool
	 */
	bool ExistBGMHandle(const std::string& setKeyName) const;
	/*
	 *	BGMハンドルの取得
	 *	@param[in]	const std::string& setKeyName	調べるBGM名
	 *	@return		int
	 */
	int GetBGMHandle(const std::string& setKeyName) const;

public:
	/*
	 *	現在のBGMの取得
	 *	@return		BGMSource& (常に持っている想定)
	 */
	inline BGMSource& GetCurrentSource() const { return *currentSource; }
};

#endif // !_BGM_MANAGER_H_
