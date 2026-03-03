/*
 *	@file	LoadBase.h
 *	@author	Seki
 */

#ifndef _LOAD_BASE_H_
#define _LOAD_BASE_H_

#include <string>
#include <memory>
#include <iostream>


/*
 *	@brief	全てのロードクラスの基底クラス
 */
class LoadBase {
protected:
	std::string filePath;		// ファイルパス
	bool isLoaded = false;		// 読み込み完了フラグ

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	const std::string& setFilePath	ファイルパス
	 */
	explicit LoadBase(const std::string& setFilePath) : filePath(setFilePath) {}
	/*
	 *	@brief		デストラクタ
	 */
	virtual ~LoadBase() = default;

public:
	/*
	 *	@brief		データのロード処理
	 */
	virtual void Load() = 0;

public:
	/*
	 *	@brief		アンロード
	 */
	inline void Unload() { isLoaded = false; }
	/*
	 *	@brief		キャッシュするリソースか判定
	 *	@return		bool
	 */
	inline virtual bool IsCash() { return false; }
	/*
	 *	ロード完了フラグ取得
	 *  return	bool
	 */
	inline virtual bool IsLoaded() const { return isLoaded; }
	/*
	 *	ファイルパス取得
	 *  return	std::string
	 */
	inline const std::string& GetPath() const { return filePath; }
};
// 別名定義
using LoadBasePtr = std::shared_ptr<LoadBase>;

#endif // !_LOAD_BASE_H_
