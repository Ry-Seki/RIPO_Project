/*
 *	@file	DungeonStageData.h
 *	@author Seki
 */

#ifndef _DUNGEON_STAGE_DATA_H_
#define _DUNGEON_STAGE_DATA_H_

#include "../Engine/JSON.h"
#include "../Engine/AutoJSONRegitry.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <json.hpp>

/* 
 *  ダンジョンステージデータ
 */
class DungeonStageData {
private:
    std::unordered_map<std::string, std::string> dungeonDataList;        // ステージデータのマップ
    
public:
    /*
     *  コンストラクタ
     */
    DungeonStageData() = default;

public:
    /*
     *  JSONデータの読み込み、mapに自動で登録する
     *  @param[in]  const JSON& setJSON     読み込むJSONデータ
     */
    void LoadFromJson(const JSON& setJSON) {
        // 自動登録クラス
        AutoJSONRegistry registry;
        // データの登録
        registry.LoadFromJson<std::string>(dungeonDataList, setJSON);
    }

public:
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
     *  キー文字列から最後の階層だけを返す
     *  @param[in]  const std::string& fullKey  検索するキー
     *  @param[out] std::string& leafName       返す最後の階層キー
     *  @return     bool
     */
    bool TryGetLeafKey(const std::string& fullKey, std::string& leafName) {
        if (fullKey.empty()) return false;  

        size_t pos = fullKey.rfind('.');
        if (pos == std::string::npos) {
            // ドットがなければキー全体が葉
            leafName = fullKey;
        }
        else {
            leafName = fullKey.substr(pos + 1);
        }
        return true;
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
};

#endif // !_DUNGEON_STAGE_DATA_H_