/*
 *	@file	SEManager.h
 *	@author	Seki
 */

#ifndef _SE_MANAGER_H_
#define _SE_MANAGER_H_

#include "../AudioSource.h"
#include "SESource.h"
#include "../../GameConst.h"

#include <DxLib.h>
#include <memory>
#include <string>
#include <list>
#include <mutex>
#include <unordered_map>

/*
 *	SEの管理クラス
 */
class SEManager {
private:
	std::unordered_map<std::string, int> seHandleMap;		// SEハンドルマップ
	std::list<std::shared_ptr<SESource>> unuseSEList;		// 未使用のSE配列
	std::list<std::shared_ptr<SESource>> useSEList;			// 使用中のSE配列
	mutable std::mutex mtx;									// スレッド安全用

public:
	/*
	 *	コンストラクタ
	 */
	SEManager() = default;
	/*
	 *	デストラクタ
	 */
	~SEManager() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize();
	/*
	 *	ロード済みのデータの設定(コールバック)
	 */
	void Setup();
	/*
	 *	更新処理
	 */
	void Update();

public:
	/*
	 *	SE再生処理
	 *  @param[in]	const std::string& setKeyName	再生するSE名
	 *  @param[in]	const int setVolume				音量
	 */
	void PlaySE(const std::string& setKeyName, const int setVolume);
	/*
	 *	SE破棄処理
	 *  @param[in]	std::shared_ptr<SESource> destroySE
	 */
	void TeardownSE(std::shared_ptr<SESource> destroySE);

public:
	/*
	 *	SEハンドルの登録
	 *	@param[in]	const std::string&	setKeyName	登録するSE名
	 *	@param[in]	const int setHandle				登録する音源ハンドル
	 */
	void RegisterSEHandle(const std::string& setKeyName, const int setHandle);
	/*
	 *	SEハンドルが登録済みか判定
	 *  @param[in]	const std::string& setKeyName	調べるSE名
	 *  @return		bool
	 */
	bool ExistSEHandle(const std::string& setKeyName) const;
	/*
	 *	SEハンドルの取得
	 *	@param[in]	const std::string& setKeyName	調べるSE名
	 *	@return		int
	 */
	int GetSEHandle(const std::string& setKeyName) const;

};
	
#endif // !_SE_MANAGER_H_
