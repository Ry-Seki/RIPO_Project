/*
 *	@file	LoadCSV.cpp
 *	@author	Seki
 */

#include "LoadCSV.h"

/*
 *	ファイルの読み込み処理
 */
void LoadCSV::Load() {
	// データのクリア
	csvData.clear();
	try {
		// ファイルの読み込み
		std::ifstream file(filePath);
		if (!file.is_open()) return;

		std::string line;
		// 1行をパース(データ構造に変換)して二次元配列に格納
		while (std::getline(file, line)) {
			auto row = ParseCSV(line);
			if (!row.empty()) {
				csvData.push_back(row);
			}
		}
		// 読み込み完了!
		isLoaded = true;
	}catch(const std::exception& exception){
		std::cerr << "[CSVロード例外] " << exception.what() << " ファイル: " << filePath << std::endl;
	}
}
/*
 *	CSV文字列の解析処理
 *	param[in]	const std::string& line		1行分の文字列
 *	return		std::vector<std::string>	パース結果（1行分）
 */
std::vector<std::string> LoadCSV::ParseCSV(const std::string& line) const {
	std::vector<std::string> row;
	std::stringstream ss(line);
	std::string cell;

	while (std::getline(ss, cell, ',')) {
		// 余分な空白・改行を削除
		if (!cell.empty() && cell.back() == '\r') {
			cell.pop_back(); // Windows改行対策
		}
		row.push_back(cell);
	}

	return row;
}