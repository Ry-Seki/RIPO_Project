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

/*
 *	SEの管理クラス
 */
class SEManager {
private:
	std::list<std::shared_ptr<SESource>> unuseSEList;		// 未使用のSE配列
	std::list<std::shared_ptr<SESource>> useSEList;		// 使用中のSE配列
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
	 *  @param[in]	const int setHandle		再生する音源ハンドル
	 *  @param[in]	const int setVolume		音量
	 */
	void PlaySE(const int setHandle, const int setVolume);
	/*
	 *	SE破棄処理
	 *  @param[in]	std::shared_ptr<SESource> destroySE
	 */
	void TeardownSE(std::shared_ptr<SESource> destroySE);

};
	
#endif // !_SE_MANAGER_H_
