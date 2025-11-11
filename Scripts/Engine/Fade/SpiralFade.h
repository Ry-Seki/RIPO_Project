/*
 *  @file   SpiralFade.h
 *  @author Seki
 */

#ifndef _SPIRAL_FADE_H_
#define _SPIRAL_FADE_H_

#include "FadeBase.h"
#include "../GameConst.h"

#include <cmath>
#include <algorithm>

 /*
  *  渦巻きフェード
  */
class SpiralFade : public FadeBase {
private:
    int color;

public:
    /*
     *  コンストラクタ
     */
    SpiralFade(float duration, FadeDirection dir, FadeMode mode, int color = GetColor(0, 0, 0))
        : FadeBase(duration, dir, mode), color(color) {}

    /*
     *  描画処理
     */
    void Render() override {
        float t = std::clamp(elapsed / duration, 0.0f, 1.0f);
        float progress = (direction == FadeDirection::In) ? (1.0f - t) : t;
        int screenW = GameConst::WINDOW_WIDTH;
        int screenH = GameConst::WINDOW_HEIGHT;
        int cx = screenW / 2, cy = screenH / 2;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(progress * 255));

        constexpr int circleCount = 300;
        const float maxRadius = std::hypotf((float)cx, (float)cy) * 1.2f;

        for (int i = 0; i < circleCount; ++i) {
            float rate = (float)i / circleCount;
            float radius = maxRadius * std::pow(rate, 0.5f) * progress;
            float angle = rate * 20.0f * 3.1415926f + progress * 8.0f * (1.0f - rate);
            int x = static_cast<int>(cx + std::cos(angle) * radius);
            int y = static_cast<int>(cy + std::sin(angle) * radius);
            int circleRadius = static_cast<int>((30 * (1.0f - rate) + 5));
            DrawCircle(x, y, circleRadius, color, TRUE);
        }

        // 全画面覆いで補強
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(progress * 255));
        DrawBox(0, 0, screenW, screenH, color, TRUE);

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};

#endif // !_SPIRAL_FADE_H_