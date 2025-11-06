/*
 *	@file	DungeonStageData.h
 *	@author Seki
 */

#ifndef _DUNGEON_STAGE_DATA_H_
#define _DUNGEON_STAGE_DATA_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <json.hpp>

using JSON = nlohmann::json;
/* 
 *  ダンジョンステージデータ
 */
class DungeonStageData {
private:
    std::unordered_map<std::string, std::string> dungeonDataList;        // ステージデータのマップ

    
public:
    DungeonStageData() = default;

public:
    /*
     *  JSONデータの読み込みをし、新しく登録された要素数を返す
     *  @return size_t
     */
    size_t LoadFromJson(const JSON& setJSON) {
        // 登録前の要素数
        size_t before = dungeonDataList.size();
        // データの登録
        parseRecursive(setJSON, "");
        // 新規で登録された数を返す
        return dungeonDataList.size() - before;
    }
    /*
     *  指定したキーに対してパスを返す
     *  @param[in]  const std::string& fullKey  検索するキー
     *  @param[out] const std::string& outPath  返すパス
     *  @return     bool
     */
    bool TryGet(const std::string& fullKey, std::string& outPath) const {
        auto it = dungeonDataList.find(fullKey);
        if (it == dungeonDataList.end()) return false;
        // 見つかった値を返す
        outPath = it->second;
        return true;
    }
    /*
     *  葉のキー名(末端にあるキー)だけで最初に見つかった値を返す(重複は考慮しない)
     *  @param[in]  const std::string& leafName     検索するキー 
     *  @param[out] const std::string& outPath      返すパス
     *  @return     bool
     */
    bool TryGetByLeafName(const std::string& leafName, std::string& outPath) const {
        for (const auto& entry : dungeonDataList) {
            // fullKeyの最後がleafNameと一致するか
            const std::string& full = entry.first;
            // 完全一致
            if (full == leafName) { 
                outPath = entry.second; 
                return true;
            }
            // 最後の階層がleafNameと一致するか
            if (full.size() > leafName.size() + 1 && full.substr(full.size() - leafName.size() - 1) == "." + leafName) {
                outPath = entry.second;
                return true;
            }
        }
        return false;
    }
    /*
     *  カテゴリーの取得
     *  @param[in] const std::string& categoryName  取得したいカテゴリー名
     */
    std::unordered_map<std::string, std::string> GetCategory(const std::string& categoryName) const {
        std::unordered_map<std::string, std::string> result;
        for (const auto& [key, value] : dungeonDataList) {
            if (key.rfind(categoryName + ".", 0) == 0) { // prefix一致
                result[key.substr(categoryName.size() + 1)] = value;
            }
        }
        return result;
    }
    /*
     *  指定したカテゴリ内の配列キーを取得
     *  @param[in]  const std::string& categoryName  カテゴリ名
     *  @param[in]  const std::string& arrayKey      配列のキー名
     *  @return     std::vector<std::string>         
     */
    std::vector<std::string> GetArray(const std::string& categoryName, const std::string& arrayKey) const {
        std::vector<std::string> result;

        // キーを探す
        const std::string prefix = categoryName + "." + arrayKey + "[";

        for (const auto& [key, value] : dungeonDataList) {
            // 配列データを探す
            if (key.rfind(prefix, 0) == 0) {
                result.push_back(value);
            }
        }
        return result;
    }
private:
    /*
     *  JSONデータを再帰的に辿り、葉の文字列データをキーとして登録する
     *  @param[in]  const JSON& node            現在処理中のJSONデータ
     *  @param[in]  const std::string& prefix   ドット区切りで記憶するための文字列
     */
    void parseRecursive(const JSON& node, const std::string& prefix) {
        // JSONがオブジェクトの場合
        if (node.is_object()) {
            for (auto& entry : node.items()) {
                // 現在のキー名
                std::string key = entry.key();
                // 現在の値
                const JSON& value = entry.value();
                // 階層付きキー
                std::string newKey = prefix.empty() ? key : prefix + "." + key;
                // 値が文字列なら登録、そうでなければ再帰的に検索
                if (value.is_string()) {
                    dungeonDataList[newKey] = value.get<std::string>();
                }else {
                    parseRecursive(value, newKey);
                }
            }
        }
        // JSONが配列の場合
        else if (node.is_array()) {
            for (size_t i = 0; i < node.size(); ++i) {
                // 配列の要素
                const JSON& element = node[i];
                // 配列用キー
                std::string newKey = prefix + "[" + std::to_string(i) + "]";
                // 要素が文字列なら登録、そうでなければ再帰的に検索
                if (element.is_string()) {
                    dungeonDataList[newKey] = element.get<std::string>();
                }else {
                    parseRecursive(element, newKey);
                }
            }
        }
        // 数値や bool 等は無視（必要なら拡張）
    }
};
#endif // !_DUNGEON_STAGE_DATA_H_
