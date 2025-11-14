/*
 *	@file	AutoJSONRegistry.h
 *	@author	Seki
 */

#ifndef _AUTO_JSON_REGISTRY_H_
#define _AUTO_JSON_REGISTRY_H_
#include "../Engine/JSON.h"

#include <unordered_map>
#include <string>
#include <type_traits>
#include <json.hpp>

/*
 *  JSON->mapへ自動登録するクラス
 */
class AutoJSONRegistry {
public:
    AutoJSONRegistry() = default;

    /*
     *  JSONを任意のmapに自動登録
     *  @param[in]  std::unordered_map<std::string, T>& map     登録対象のmap
     *  @param[in]  const JSON& setJSON                         読み込むJSONデータ
     */
    template<typename T>
    void LoadFromJson(std::unordered_map<std::string, T>& map, const JSON& setJSON) {
        ParseRecursive<T>(map, setJSON, "");
    }

private:
    /*
     *  JSONデータを再帰的に探索して、mapに自動登録する
     *  @param[in]  std::unordered_map<std::string, T>& map     登録対象のmap
     *  @param[in]  const JSON& setJSON                         読み込むJSONデータ
     *  @param[in]  const std::string& prefix                   作成済みのキー(再帰用)
     */
    template<typename T>
    void ParseRecursive(std::unordered_map<std::string, T>& map, const JSON& setJSON, const std::string& prefix) {
        // ノードの中身によって処理を変える
        if (setJSON.is_object()) {
            for (auto& entry : setJSON.items()) {
                // 現在の階層キーに新しいキーを追加
                std::string newKey = prefix.empty() ? entry.key() : prefix + "." + entry.key();
                // 再帰的に検索
                ParseRecursive(map, entry.value(), newKey);
            }
        } else if (setJSON.is_array()) {
            // ノードが配列の場合
            for (size_t i = 0; i < setJSON.size(); ++i) {
                // 配列のインデックスをキーに追加
                std::string newKey = prefix + "[" + std::to_string(i) + "]";
                // 再帰的に検索
                ParseRecursive(map, setJSON[i], newKey);
            }
        } else {
            // ノードがリーフの場合、型によってコンパイル時に分岐
            if constexpr (std::is_same_v<T, std::string>) {
                if (setJSON.is_string()) map[prefix] = setJSON.get<std::string>();
            }
            else if constexpr (std::is_same_v<T, int>) {
                if (setJSON.is_number_integer()) map[prefix] = setJSON.get<int>();
            }
            else if constexpr (std::is_same_v<T, float>) {
                if (setJSON.is_number_float()) map[prefix] = setJSON.get<float>();
            }
            else if constexpr (std::is_same_v<T, bool>) {
                if (setJSON.is_boolean()) map[prefix] = setJSON.get<bool>();
            }
            // 他の型は無視
        }
    }
};

#endif // !_AUTO_JSON_REGISTRY_H_