/*
 *	@file	LoadManager.h
 *	@author	Seki
 */

#ifndef _LOAD_MANAGER_H_
#define _LOAD_MANAGER_H_

#include "../Singleton.h"
#include "LoadSystem.h"
#include "LoadRegistry.h"

#include <functional>
#include <memory>
#include <queue>

/*
 *	@brief  ファイル読み込み管理クラス（多段階ロード対応版）
 */
class LoadManager : public Singleton<LoadManager> {
    // フレンド宣言
    friend class Singleton<LoadManager>;

private:
    LoadSystem system;                     // ロードの内部処理(常に持っている想定)
    std::queue<std::function<void()>> onCompleteQueue;      // コールバックキュー
    LoadRegistry loadRegistry;                              // リソースキャッシュ

private:
    /*
     *  @brief  コンストラクタ
     */
    LoadManager() = default;
    /*
     *  @brief  デストラクタ
     */
    ~LoadManager() override = default;

public:
    /*
     *  @brief  更新処理
     */
    void Update(float unscaledDeltaTime);
    /*
     *  @brief  描画処理
     */
    void Render();

public:
    /*
     *  @brief      リソースの読み込み(コールバックの設定込み)
     *  @param[in]  const std::string& setFilePath      ファイルパス
     *  @return     std::shared_ptr<T> TはLoadBasePtrの派生クラス
     */
    template<class T, typename = std::enable_if_t<std::is_base_of_v<LoadBase, T>>>
    std::shared_ptr<T> LoadResource(const std::string& setFilePath) {
        if (setFilePath.empty()) return nullptr;
        auto loader = std::make_shared<T>(setFilePath);
        if (loader->IsCash()) {
            // registryから取得
            auto cached = loadRegistry.Get(setFilePath);
            // キャッシュが存在していればキャッシュを、無ければ登録する
            if (cached) {
                auto resource = std::dynamic_pointer_cast<T>(cached);
                return resource;
            } else {
                loadRegistry.Register(setFilePath, loader);
            }
        }
        // ロードリストに登録
        system.AddLoader(loader);
        system.ResetCompleteFlag();

        return loader;
    }
    /*
     *  @brief  コールバック登録
     */
    void SetOnComplete(const std::function<void()>& callback);
    /*
     *  @brief  コールバックの実行
     */
    void ActiveCallback();
    /*
     *  @brief  キャッシュのクリア
     */
    void Clear();

public:
    /*
     *  @brief  ロードアニメーションに追加
     */
    inline void AddAnimation(const LoadAnimationPtr& animation) {
        system.AddAnimation(animation);
    }
    /*
     *  @brief  ロード中判定
     */
    inline bool IsLoading() const {
        return !system.IsComplete();
    }
    /*
     *  @brief  ロードシステムのロード状況をリセット
     */
    inline void ClearLoadSystem() { system.Clear(); }
};

#endif // !_LOAD_MANAGER_H_