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

class LoadSystem {
private:
    std::vector<LoadBasePtr> loadList;              // ロードする配列
    std::vector<LoadAnimationPtr> animationList;    // ロード中のアニメーションリスト
    int currentIndex = 0;                           // 現在の要素数
    bool isComplete = false;                        // ロード完了フラグ

public:
    /*
     *  コンストラクタ
     */
    LoadSystem() = default;
    /*
     *  デストラクタ
     */
    ~LoadSystem() { 
        Clear();
        for (auto& anim : animationList) {
            if (anim) anim->Unload();
        }
        animationList.clear();
    }

public:
    /*
     *  更新処理
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
     *  描画処理
     */
    void Render() {
        for (auto& anim : animationList) {
            anim->Render();
        }
    }
    /*
     *  ロードリストに追加
     */
    void AddLoader(const LoadBasePtr& loader) {
        loadList.push_back(loader);
    }
    /*
     *  アニメーションリストに追加
     */
    void AddAnimation(const LoadAnimationPtr& anim) {
        animationList.push_back(anim);
    }
    /*
     *  メンバ変数のリセット
     */
    void Clear() {
        isComplete = false;
        loadList.clear();
        currentIndex = 0;
    }

public:
    /*
     *  ロード中フラグの取得
     *  @return     bool
     */
    inline bool IsLoading() const { return !isComplete && !loadList.empty(); }
    /*
     *  ロード完了フラグの取得
     *  @return     bool
     */
    inline bool IsComplete() const { return isComplete || loadList.empty(); }
    /*
     *  ロード進捗度の取得
     *  @return     float
     */
    inline float GetProgress() const {
        return loadList.empty() ? 1.0f : float(currentIndex) / float(loadList.size());
    }
    /*
     *  ロード完了フラグの解除
     */
    inline void ResetCompleteFlag() { isComplete = false; }
    /*
     *  ロード完了フラグの適応
     */
    inline void CompleteLoading() { isComplete = true; }
};

#endif // !_LOAD_SYSTEM_H_
