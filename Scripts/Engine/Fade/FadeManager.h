/*
 *  @file   FadeManager.h
 *  @author Seki
 */

#ifndef _FADE_MANAGER_H_
#define _FADE_MANAGER_H_

#include "FadeBase.h"
#include "../Singleton.h"

#include <functional>

/*
 *  @brief  フェード管理処理
 */
class FadeManager : public Singleton<FadeManager>{
    // フレンド宣言
    friend class Singleton<FadeManager>;
private:
    FadeBasePtr currentFade;
    std::function<void()> FadeCallback; // フェードのコールバック

private:
    /*
     *  コンストラクタ
     */
    FadeManager() = default;
    /*
     *  デストラクタ
     */
    ~FadeManager() = default;

public:
    /*
     *  @brief      フェード開始処理
     *  @param[in]  const FadeBasePtr& setFade
     *  @param[in]  std::function<void()> callback = nullptr
     */
    void StartFade(const FadeBasePtr& setFade, std::function<void()> callback = nullptr);
    /*
     *  @brief      更新処理
     */
    void Update(float unscaledDeltaTime);
    /*
     *  @brief      描画処理
     */
    void Render();

public:
    /*
     *  @brief      フェードモード取得
     *  @return     FadeMode
     */
    inline FadeMode GetMode() const {
        return currentFade ? currentFade->GetMode() : FadeMode::NonStop;
    }
    /*
     *  @brief      フェード中かどうか
     *  @return     bool
     */
    inline bool IsFading() const {
        return currentFade != nullptr;
    }
}; 
#endif // !_FADE_MANAGER_H_