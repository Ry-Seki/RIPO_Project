/*
 *	@file	LoadBinary.h
 *	@author	Seki
 */

#ifndef _LOAD_BINARY_H_
#define _LOAD_BINARY_H_

#include "../LoadBase.h"
#include <fstream>
#include <vector>

/*
 *	バイナリファイル読み込みクラス
 */
class LoadBinary : public LoadBase {
private:
	std::vector<unsigned char> binaryData;		// バイナリファイル

public:
	/*
	 *	コンストラクタ
	 *	@param[in]	const std::string& setFilePath	ファイルパス
	 */
	explicit LoadBinary(const std::string& setFilePath) : LoadBase(setFilePath) {}

public:
	/*
	 *	読み込み処理
	 */
	void Load() override;

public:
	/*
	 *	バイナリデータの取得
	 *  return	const std::vector<unsigned char>
	 */
	inline const std::vector<unsigned char>& GetData() const { return binaryData; }
};

#endif // !_LOAD_BINARY_H_
