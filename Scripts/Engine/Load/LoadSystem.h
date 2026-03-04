/*
 *	@file	LoadSystem.h
 *	@author	Seki
 */

#ifndef _LOAD_SYSTEM_H_
#define _LOAD_SYSTEM_H_

#include "LoadBase.h"
#include "Animation/LoadAnimationBase.h"
#include <vector>
#include <memory>

/*
 *  @brief  ロード実行処理
 */
class LoadSystem {
private:
    std::vector<LoadBasePtr> loadList;              // ロードする配列
    std::vector<LoadAnimationPtr> animationList;    // ロード中のアニメーションリスト
    int currentIndex = 0;                           // 現在の要素数
    bool isComplete = false;                        // ロード完了フラグ

public:
    /*
     *  @brief  コンストラクタ
     */
    LoadSystem() = default;
    /*
     *  @brief  デストラクタ
     */
    ~LoadSystem() { 
        Teardown();
    }

public:
    /*
     *  @brief  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (loadList.empty()) {
            isComplete = true;
            return;
        }
        // ロードアニメーションの更新
        for (auto& anim : animationList) {
            if (anim) anim->Update(unscaledDeltaTime);
        }
        // ロード処理
        if (currentIndex < loadList.size()) {
            loadList[currentIndex]->Load();
            currentIndex++;
        }

        if (currentIndex >= loadList.size()) {
            isComplete = true;
        }
    }
    /*
     *  @brief  描画処理
     */
    void Render() {
        for (auto& anim : animationList) {
            anim->Render();
        }
    }
    /*
     *  @brief  片付け処理
     */
    void Teardown() {
        Clear();
        for (auto& anim : animationList) {
            if (anim) anim->Unload();
        }
        animationList.clear();
    }
    /*
     *  @brief      ロードリストに追加
     *  @param[in]  const LoadBasePtr& loader
     */
    void AddLoader(const LoadBasePtr& loader) {
        loadList.push_back(loader);
    }
    /*
     *  @brief      アニメーションリストに追加
     *  @param[in]  const LoadAnimationPtr& anim
     */
    void AddAnimation(const LoadAnimationPtr& anim) {
        animationList.push_back(anim);
    }
    /*
     *  @brief  メンバ変数のリセット
     */
    void Clear() {
        isComplete = false;
        loadList.clear();
        currentIndex = 0;
    }

public:
    /*
     *  @brief      ロード中判定
     *  @return     bool
     */
    inline bool IsLoading() const { return !isComplete && !loadList.empty(); }
    /*
     *  @brief      ロード完了判定
     *  @return     bool
     */
    inline bool IsComplete() const { return isComplete || loadList.empty(); }
    /*
     *  @brief      ロード完了フラグの解除
     */
    inline void ResetCompleteFlag() { isComplete = false; }
    /*
     *  @brief      ロード完了フラグの適応
     */
    inline void CompleteLoading() { isComplete = true; }
};

#endif // !_LOAD_SYSTEM_H_