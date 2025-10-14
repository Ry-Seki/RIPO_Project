/*
 *  @file   FadeBase.h
 *  @author Seki
 */

#ifndef _FADE_BASE_H_
#define _FADE_BASE_H_

#include <memory>
#include <DxLib.h>

enum class FadeMode {
    Stop,       // フェード中はSceneやGameObjectのUpdateを止める
    NonStop     // フェード中もUpdateを継続
};

enum class FadeDirection {
    In,         // フェードイン
    Out         // フェードアウト
};

/*
 *  フェードの基底処理
 */
class FadeBase {
protected:
    float duration;       // フェード時間（秒）
    float elapsed = 0.0f; // 経過時間
    FadeDirection direction;
    FadeMode mode;
    bool isFinished = false;

public:
    /*
     *  コンストラクタ
     *  @param[in]  float         duration  フェード時間
     *  @param[in]  FadeDirection dir       フェードアウト・フェードインの選択
     *  @param[in]  FadeMode      mode      フェード中、Sceneの更新を止めるか
     */
    FadeBase(float duration, FadeDirection dir, FadeMode mode)
        : duration(duration), direction(dir), mode(mode) {}
    /*
     *  デストラクタ
     */
    virtual ~FadeBase() = default;

public:
    /*
     *  更新処理
     */
    virtual void Update(float deltaTime) {
        if (isFinished) return;
        elapsed += deltaTime;
        if (elapsed >= duration) {
            elapsed = duration;
            isFinished = true;
        }
    }
    /*
     *  描画処理
     */
    virtual void Render() = 0; // DxLib 描画
    /*
     *  リセット処理
     */
    void Reset(FadeDirection dir) {
        elapsed = 0.0f;
        direction = dir;
        isFinished = false;
    }
    /*
     *  終了フラグの取得
     */
    bool IsFinished() const { return isFinished; }
    /*
     *  モードの取得
     */
    FadeMode GetMode() const { return mode; }
}; 
// 別名定義
using FadeBasePtr = std::shared_ptr<FadeBase>;

#endif // !_FADE_BASE_H_
