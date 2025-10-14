/*
 *	@file	LoadBase.h
 *	@author	Seki
 */

#ifndef _LOAD_BASE_H_
#define _LOAD_BASE_H_

#include <string>
#include <memory>

/*
 *	全てのロードクラスの基底クラス
 */
class LoadBase {
protected:
	std::string filePath;		// ファイルパス
	bool isLoaded = false;		// 読み込み完了フラグ

public:
	/*
	 *	コンストラクタ
	 *	param[in]	const std::string& setFilePath	ファイルパス
	 */
	explicit LoadBase(const std::string& setFilePath) : filePath(setFilePath) {}
	/*
	 *	デストラクタ
	 */
	virtual ~LoadBase() = default;

public:
	/*
	 *	データのロード処理
	 */
	virtual void Load() = 0;
	
public:
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
