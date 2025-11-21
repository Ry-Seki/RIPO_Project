/*
 *	@file	SaveDataManager.h
 */

#ifndef _SAVE_DATA_MANAGER_H_
#define _SAVE_DATA_MANAGER_H_

#include "../Singleton.h"
#include "../JSON.h"

class SaveDataManager : public Singleton<SaveDataManager> {
	friend class Singleton<SaveDataManager>;

private:
	JSON saveData;

private:
	/*
	 *	@brief	コンストラクタ
	 */
	SaveDataManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SaveDataManager() override {}

public:
	/*
	 *	@brief	セーブデータの作成
	 */

	/*
	 *	@brief	セーブデータの読み込み処理(3つ + 1つ<オートセーブ用>)
	 */

	/*
	 *	@brief	セーブデータの書き込み処理
	 */

public:
	/*
	 *	@brief	選択されたセーブデータの取得
	 *	@return	std::unique_ptr
	 */

};
#endif // !_SAVE_DATA_MANAGER_H_