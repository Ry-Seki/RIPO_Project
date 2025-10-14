/*
 *	@file	LoadBinary.cpp
 *	@author	Seki
 */

#include "LoadBinary.h"

/*
 *	ファイルの読み込み処理
 */
void LoadBinary::Load() {
    // データのクリア
    binaryData.clear();
    try {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())	return;

        // ファイルサイズを取得
        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        // バッファ確保
        binaryData.resize(size);
        // データ読み込み
        file.read(reinterpret_cast<char*>(binaryData.data()), size);
        file.close();

        // 読み込み完了
        isLoaded = true;
    }catch(const std::exception& exception){
        std::cerr << "[バイナリロード例外] " << exception.what() << " ファイル: " << filePath << std::endl;
    }
}
