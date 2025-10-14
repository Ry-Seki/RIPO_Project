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
#include <functional>

/*
 *	ファイル読み込み管理クラス
 */
class LoadManager : public Singleton<LoadManager> {
private:
	std::vector<LoadBasePtr> loadList;	// 読み込むファイルリスト
	int currentIndex = 0;				// ロードのインデクス番号
	bool isComplete = false;			// 読み込み完了フラグ
	std::function<void()> onComplete;	// ロード終了時のコールバック

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
		if (currentIndex >= loadList.size()) {
			isComplete = true;

			// コールバックの呼び出し
			if (onComplete) {
				onComplete();
				onComplete = nullptr;
			}
		}
    }

public:
	/*
	 *	読み込み対象を追加
	 *	param[in]	const LoadBasePtr& load		追加するファイル
	 */
	inline void AddLoader(const LoadBasePtr& load) {
		loadList.push_back(load);
	}
	/*
	 *	ロード完了時コールバックの設定
	 */
	inline void SetOnComplete(const std::function<void()>& callback) {
		onComplete = callback;
	}
	/*
	 *	ロードリストの初期化
	 */
	inline void Clear() {
		loadList.clear();
		currentIndex = 0;
		isComplete = false;
		onComplete = nullptr;
	}

public:
	/*
	 *	読み込み中フラグの取得
	 *	return	bool
	 */
	inline bool IsLoading() const { return !isComplete && !loadList.empty(); }
	/*
	 *	読み込み完了フラグの取得
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