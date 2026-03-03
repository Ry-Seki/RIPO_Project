/*
 *	@file	LoadRegistry.h
 *	@author	Seki
 */

#ifndef _LOAD_REGISTRY_H_
#define _LOAD_REGISTRY_H_

#include "LoadBase.h"

#include <unordered_map>
#include <memory>
#include <string>

/*
 *  @brief  同じリソースの二重ロードを防ぐ
 *          キャッシュから既存リソースを取得可能
 */
class LoadRegistry {
private:
    std::unordered_map<std::string, LoadBasePtr> registry; // キー → ロード済みリソース

public:
    /*
     *  @brief  コンストラクタ
     */
    LoadRegistry() = default;
    /*
     *  @brief  デストラクタ
     */
    ~LoadRegistry() {
		Clear();
    }

public:
    /*
     *  @brief      リソースを登録
     */
    inline void Register(const std::string& key, const LoadBasePtr& resource) {
        if (!resource) return;
        // キーを探す
        if (registry.find(key) == registry.end()) registry[key] = resource;
    }
    /*
     *  @brief      登録済みかを確認
     *  @return     bool
     */ 
    inline bool Exists(const std::string& key) const {
        return registry.find(key) != registry.end();
    }
    /*
     *  @brief      登録済みリソースを取得
     *  @return     LoadBasePtr
     */
    inline LoadBasePtr Get(const std::string& key) const {
        auto it = registry.find(key);
        if (it != registry.end()) return it->second;
        return nullptr;
    }
    /*
     *  @brief  キャッシュ全削除
     */
    inline void Clear() {
        registry.clear();
    }
};

#endif // !_LOAD_REGISTRY_H_