/*
 *	@file	LoadCSV.cpp
 *	@author	Seki
 */

#include "LoadCSV.h"

/*
 *	ファイルの読み込み処理
 */
void LoadCSV::Load() {
	// 前回データのクリア
	csvData.clear();
	// ファイルの読み込み
	std::ifstream file(filePath);
	if (!file.is_open()) return;

	std::string line;
	// 一行ずつ読み込む
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;

		// セルごとに読み込む
		while (std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}
		// 行を追加する
		csvData.push_back(row);
	}
	// 読み込み完了
	isLoaded = true;
}
