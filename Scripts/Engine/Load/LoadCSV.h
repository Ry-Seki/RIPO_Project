/*
 *	@file	LoadCSV.h
 *  @author	Seki
 */

#ifndef _LOAD_CSV_H_
#define _LOAD_CSV_H_

#include "LoadBase.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/*
 *	CSVファイルの読み込みクラス
 */
class LoadCSV : public LoadBase {
private:
	std::vector<std::vector<std::string>> csvData;	// CSVデータ(行、列)二次元配列

public:
	/*
	 *	コンストラクタ
	 *	param[in]	const std::string& setFilePath		ファイルパス
	 */
	explicit LoadCSV(const std::string& setFilePath) : LoadBase(setFilePath) {}

public:
	/*
	 *	読み込み処理
	 */
	void Load() override;

public:
	/*
	 *	CSVデータの取得
	 *  return	const std::vector<std::vector<std::string>>
	 */
	inline const std::vector<std::vector<std::string>>& GetData() const { return csvData; }
};

#endif // !_LOAD_CSV_H_
