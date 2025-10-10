#ifndef _FADEMANAGER_H_
#define _FADEMANAGER_H_

#include "FadeBase.h"
#include "../Singleton.h"

#include <memory>
#include <functional>

class FadeManager : public Singleton<FadeManager>{
private:
    FadeBasePtr currentFade;
    std::function<void()> onFadeFinished; // フェード終了時のコールバック

public:
    FadeManager() = default;
    ~FadeManager() = default;

public:
    // フェード開始
    void StartFade(const std::shared_ptr<FadeBase>& setFade, std::function<void()> callback = nullptr) {
        currentFade = setFade;
        onFadeFinished = callback;
    }

    // 毎フレーム更新（TimeScale非依存）
    void Update(float unscaledDeltaTime) {
        if (!currentFade) return;

        currentFade->Update(unscaledDeltaTime);

        // フェード終了時
        if (currentFade->IsFinished()) {
            if (onFadeFinished) onFadeFinished(); // コールバック呼び出し
            currentFade.reset();                  // 自動破棄
            onFadeFinished = nullptr;
        }
    }

    // 描画
    void Render() {
        if (currentFade) currentFade->Render();
    }

    // フェードモード取得（停止 / 非停止）
    FadeMode GetMode() const {
        return currentFade ? currentFade->GetMode() : FadeMode::NonStop;
    }

    // フェード中かどうか
    bool IsFading() const {
        return currentFade != nullptr;
    }
}; 
#endif // !_FADEMANAGER_H_