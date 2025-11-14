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

class AutoJSONRegistry {
public:
    AutoJSONRegistry() = default;

    /*
     * JSONを任意のmapに自動登録
     */
    template<typename T>
    void LoadFromJson(std::unordered_map<std::string, T>& map, const JSON& json) {
        ParseRecursive<T>(map, json, "");
    }

private:
    /*
     *  JSONデータを再帰的に探索して、mapに自動登録する
     *  @param[in]  std::unordered_map<std::string, T>& map     登録対象のmap
     *  @param[in]  const JSON& node                            読み込むJSONデータ
     *  @param[in]  const std::string& prefix                   作成済みのキー
     */
    template<typename T>
    void ParseRecursive(std::unordered_map<std::string, T>& map, const JSON& node, const std::string& prefix) {
        // ノードの中身によって処理を変える
        if (node.is_object()) {
            for (auto& entry : node.items()) {
                // 現在の階層キーに新しいキーを追加
                std::string newKey = prefix.empty() ? entry.key() : prefix + "." + entry.key();
                // 再帰的に検索
                ParseRecursive(map, entry.value(), newKey);
            }
        } else if (node.is_array()) {
            // ノードが配列の場合
            for (size_t i = 0; i < node.size(); ++i) {
                // 配列のインデックスをキーに追加
                std::string newKey = prefix + "[" + std::to_string(i) + "]";
                // 再帰的に検索
                ParseRecursive(map, node[i], newKey);
            }
        } else {
            // ノードがリーフの場合、Tに応じて型チェックして登録
            if constexpr (std::is_same_v<T, std::string> && node.is_string()) {
                map[prefix] = node.get<std::string>();
            }
            else if constexpr (std::is_same_v<T, int> && node.is_number_integer()) {
                map[prefix] = node.get<int>();
            }
            else if constexpr (std::is_same_v<T, float> && node.is_number_float()) {
                map[prefix] = node.get<float>();
            }
            else if constexpr (std::is_same_v<T, bool> && node.is_boolean()) {
                map[prefix] = node.get<bool>();
            }
            // 他の型は無視
        }
    }
};

#endif // !_AUTO_JSON_REGISTRY_H_