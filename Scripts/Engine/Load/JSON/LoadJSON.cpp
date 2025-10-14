/*
 *	@file	LoadJSON.cpp
 *	@author	Seki
 */

#include "LoadJSON.h"

/*
 *	読み込み処理
 */
void LoadJSON::Load() {
	try {
		// ファイルの読み込み
		std::ifstream file(filePath);
		if (!file.is_open()) return;
		// JSONとして解析
		file >> jsonData;
		// 読み込み完了
		isLoaded = true;
	}catch(const JSON::parse_error& error){
		std::cerr << "[JSONパースエラー] " << error.what() << " ファイル: " << filePath << std::endl;
	}catch (const std::exception& exception) {
		std::cerr << "[JSONロード例外] " << exception.what() << " ファイル: " << filePath << std::endl;
	}
}
