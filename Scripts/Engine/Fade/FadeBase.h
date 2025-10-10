/*
 *  @file   FadeBase.h
 *  @author Seki
 */

#ifndef _FADE_BASE_H_
#define _FADE_BASE_H_

#include <DxLib.h>

enum class FadeMode {
    Stop,       // フェード中はSceneやGameObjectのUpdateを止める
    NonStop     // フェード中もUpdateを継続
};

enum class FadeDirection {
    In,         // フェードイン
    Out         // フェードアウト
};

class FadeBase {
protected:
    float duration;       // フェード時間（秒）
    float elapsed = 0.0f; // 経過時間
    FadeDirection direction;
    FadeMode mode;
    bool isFinished = false;

public:
    FadeBase(float duration, FadeDirection dir, FadeMode mode)
        : duration(duration), direction(dir), mode(mode) {}

    virtual ~FadeBase() = default;

    virtual void Update(float deltaTime) {
        if (isFinished) return;
        elapsed += deltaTime;
        if (elapsed >= duration) {
            elapsed = duration;
            isFinished = true;
        }
    }

    virtual void Render() = 0; // DxLib 描画
    bool IsFinished() const { return isFinished; }
    FadeMode GetMode() const { return mode; }
}; 
// 別名定義
using FadeBasePtr = std::shared_ptr<FadeBase>;

#endif // !_FADE_BASE_H_
