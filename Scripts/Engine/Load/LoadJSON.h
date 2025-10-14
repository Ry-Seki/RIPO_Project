/*
 *	MIT License | https://github.com/nlohmann/json/releases
 */

/*
 *	@file	LoadJSON.h
 *	@author	Seki
 */

#ifndef _LOAD_JSON_H_
#define _LOAD_JSON_H_

#include "LoadBase.h"
#include <fstream>
#include <json.hpp>

 // 別名定義
using JSON = nlohmann::json;

/*
 *	JSONファイルの読み込みクラス
 */
class LoadJSON : public LoadBase {
private:
	JSON jsonData;	// JSONデータ

public:
	/*
	 *	コンストラクタ
	 *	param[in]	const std::string& setFilePath		ファイルパス
	 */
	explicit LoadJSON(const std::string& setFilePath) : LoadBase(setFilePath){}

public:
	/*
	 *	読み込み処理
	 */
	void Load() override;

public:
	/*
	 *	JSONデータの取得
	 *  return JSON
	 */
	const JSON& GetData() const { return jsonData; }
};

#endif // !_LOAD_JSON_H_
