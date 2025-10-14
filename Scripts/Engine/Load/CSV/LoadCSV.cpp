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
		// 一行ずつ読み込む
		while (std::getline(file, line)) {
			std::vector<std::string> row;
			std::stringstream ss(line);
			std::string cell;

			// セルごとに読み込む
			while (std::getline(ss, cell, ',')) {
				row.push_back(cell);
			}
			// 行を追加する
			csvData.push_back(row);
		}
		// 読み込み完了
		isLoaded = true;
	}catch(const std::exception& exception){
		std::cerr << "[CSVロード例外] " << exception.what() << " ファイル: " << filePath << std::endl;
	}
}
