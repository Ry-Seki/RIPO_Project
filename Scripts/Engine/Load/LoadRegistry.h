/*
 *	@file	LoadRegistry.h
 *	@author	Seki
 */

#ifndef _LOAD_REGISTRY_H_
#define _LOAD_REGISTRY_H_

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

 // 前方宣言
class LoadBase;
using LoadBasePtr = std::shared_ptr<LoadBase>;

/*
 *  同じリソースの二重ロードを防ぐ
 *  キャッシュから既存リソースを取得可能
 */
class LoadRegistry {
private:
    std::unordered_map<std::string, LoadBasePtr> registry; // キー → ロード済みリソース
    mutable std::mutex mtx;                                // スレッド安全用

public:
    LoadRegistry() = default;
    ~LoadRegistry() = default;

public:
    /*
     *   リソースを登録
     */
    void Register(const std::string& key, const LoadBasePtr& resource) {
        if (!resource) return;
        std::lock_guard<std::mutex> lock(mtx);
        if (registry.find(key) == registry.end()) {
            registry[key] = resource;
        }
    }
    /*
     *  登録済みかを確認
     */ 
    bool Exists(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mtx);
        return registry.find(key) != registry.end();
    }
    /*
     *  登録済みリソースを取得
     */
    LoadBasePtr Get(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = registry.find(key);
        if (it != registry.end()) return it->second;
        return nullptr;
    }
    /*
     *  キャッシュ全削除
     */
    void Clear() {
        std::lock_guard<std::mutex> lock(mtx);
        registry.clear();
    }
};

#endif // !_LOAD_REGISTRY_H_