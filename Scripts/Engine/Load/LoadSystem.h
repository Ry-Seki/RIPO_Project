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
    std::vector<LoadBasePtr> loadList;
    std::vector<LoadAnimationPtr> animationList;
    int currentIndex = 0;
    bool isComplete = false;

public:
    LoadSystem() = default;
    ~LoadSystem() { Clear(); }

    void Update(float unscaledDeltaTime) {
        if (isComplete || loadList.empty()) return;

        if (currentIndex < loadList.size()) {
            loadList[currentIndex]->Load();
            currentIndex++;
        }

        for (auto& anim : animationList) {
            if (anim) anim->Update(unscaledDeltaTime);
        }

        if (currentIndex >= loadList.size()) {
            isComplete = true;
        }
    }

    void Render() {
        for (auto& anim : animationList) {
            anim->Render();
        }
    }

    void AddLoader(const LoadBasePtr& loader) {
        loadList.push_back(loader);
    }

    void AddAnimation(const LoadAnimationPtr& anim) {
        animationList.push_back(anim);
    }

    void Clear() {
        loadList.clear();
        for (auto& anim : animationList) {
            if (anim) anim->Unload();
        }
        animationList.clear();
        currentIndex = 0;
        isComplete = false;
    }

    inline bool IsLoading() const { return !isComplete && !loadList.empty(); }
    inline bool IsComplete() const { return isComplete || loadList.empty(); }
    inline float GetProgress() const {
        return loadList.empty() ? 1.0f : float(currentIndex) / float(loadList.size());
    }
    inline void ResetCompleteFlag() { isComplete = false; }
};

#endif // !_LOAD_SYSTEM_H_
