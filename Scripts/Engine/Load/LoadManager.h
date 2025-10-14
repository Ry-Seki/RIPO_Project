/*
 *	@file	LoadManager.h
 *	@author	Seki
 */

#ifndef _LOAD_MANAGER_H_
#define _LOAD_MANAGER_H_

#include "../Singleton.h"
#include "LoadBase.h"

#include <vector>
#include <memory>

/*
 *	ファイル読み込み管理クラス
 */
class LoadManager : public Singleton<LoadManager> {
private:
	std::vector<LoadBasePtr> loadList;	// 読み込むファイルリスト
	int currentIndex = 0;				// ロードのインデクス番号
	bool isComplete = false;			// 読み込み完了フラグ

public:
	/*
	 *	コンストラクタ
	 */
	LoadManager() = default;
	/*
	 *	デストラクタ
	 */
	~LoadManager() = default;

public:
	/*
	 *	更新処理
	 */
    void Update() {
		if (isComplete || loadList.empty()) return;

		// 現在のファイルを読み込む
		if (currentIndex < loadList.size()) {
			loadList[currentIndex]->Load();
			currentIndex++;
		}
		// 読み込み完了
		if (currentIndex >= loadList.size()) isComplete = true;
    }

public:
	/*
	 *	読み込み対象を追加
	 *	param[in]	const LoadBasePtr& load		追加するファイル
	 */
	inline void AddLoder(const LoadBasePtr& load) {
		loadList.push_back(load);
	}
	/*
	 *	ロードリストの初期化
	 */
	inline void Clear() {
		loadList.clear();
		currentIndex = 0;
		isComplete = false;
	}
	/*
	 *	読み込みフラグの取得
	 *  return	bool
	 */
	inline bool IsComplete() const { return isComplete; }
	/*
	 *	読み込み進捗
	 *  return	float
	 */
	inline float GetProgress() const {
		if (loadList.empty()) return 1.0f;
		return static_cast<float>(currentIndex) / static_cast<float>(loadList.size());
	}
};
#endif // !_LOAD_MANAGER_H_
