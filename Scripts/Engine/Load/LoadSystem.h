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
 *	ロードの内部処理
 */
class LoadSystem {
private:
    std::vector<LoadBasePtr> loadList;
    std::vector<LoadAnimationPtr> animationList;
    int currentIndex = 0;
    bool isComplete = false;

public:
    /*
     *  コンストラクタ
     */
    LoadSystem() = default;
    /*
     *  デストラクタ
     */
    ~LoadSystem() { Clear(); }

public:
    /*
     *  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (isComplete || loadList.empty()) return;

        // 現在のファイルを読み込む
        if (currentIndex < loadList.size()) {
            loadList[currentIndex]->Load();
            currentIndex++;
        }

        // アニメーション更新
        for (auto& anim : animationList) {
            if (anim) anim->Update(unscaledDeltaTime);
        }

        // 読み込み完了判定
        if (currentIndex >= loadList.size()) {
            isComplete = true;
        }
    }
    /*
     *  描画処理
     */
    void Render() {
        for (auto& anim : animationList) anim->Render();
    }
    
public:
    /*
     *  ロードリストに追加
     */
    void AddLoader(const LoadBasePtr& loader) {
        loadList.push_back(loader);
    }
    /*
     *  ロードアニメーションリストに追加
     */
    void AddAnimation(const LoadAnimationPtr& anim) {
        animationList.push_back(anim);
    }
    /*
     *  解放処理
     */
    void Clear() {
        loadList.clear(); 
        for (auto& anim : animationList) {
            if (anim) anim->Unload();
        }
        animationList.clear(); 
        currentIndex = 0; 
        isComplete = false; 
    }

public:
    /*
     *  ロード中フラグの取得
     */
    bool IsLoading() const { return !isComplete && !loadList.empty(); }
    /*
     *  ロード完了フラグの取得
     */
    bool IsComplete() const { return isComplete; }
    /*
     *  ロード進捗度の取得
     */
    float GetProgress() const { return loadList.empty() ? 1.0f : float(currentIndex) / float(loadList.size()); }
};

#endif // !_LOAD_SYSTEM_H_
