#ifndef _FADEMANAGER_H_
#define _FADEMANAGER_H_
#include <memory>
#include "FadeBase.h"
#include <functional>

class FadeManager {

private:
    FadeBasePtr currentFade;
    std::function<void()> onFadeFinished; // フェード終了時のコールバック

    // private コンストラクタ：外部生成禁止
    FadeManager() = default;

public:
    // コピー禁止
    FadeManager(const FadeManager&) = delete;
    FadeManager& operator=(const FadeManager&) = delete;

    ~FadeManager() = default;

    // 唯一のインスタンス取得（スレッドセーフ）
    static FadeManager& Instance() {
        static FadeManager instance;
        return instance;
    }

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