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
        // 進行率を正規化
        float t = std::clamp(elapsed / duration, 0.0f, 1.0f);

        // direction に応じて進行を反転
        float progress = (direction == FadeDirection::In) ? (1.0f - t) : t;

        // アルファ計算
        int alpha = static_cast<int>(progress * 255);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // 画面中央
        const int cx = GameConst::WINDOW_WIDTH / 2;
        const int cy = GameConst::WINDOW_HEIGHT / 2;

        // 渦巻きパラメータ
        constexpr int circleCount = 300;     // 円の数
        constexpr float spiralSpeed = 0.3f;  // 渦の回転スピード
        const float maxRadius = std::hypotf((float)cx, (float)cy) * 1.2f;

        // --------------------------------------
        // フェードアウト → 渦が外へ広がる
        // フェードイン → 渦が中心に巻き込まれる
        // --------------------------------------
        for (int i = 0; i < circleCount; ++i) {
            float rate = (float)i / circleCount;
            float angle = rate * 10.0f * 3.1415926f + (progress * 6.2831f * spiralSpeed);
            float radius = rate * maxRadius * progress;

            int x = static_cast<int>(cx + std::cos(angle) * radius);
            int y = static_cast<int>(cy + std::sin(angle) * radius);

            // 小さい円を多数描画して渦を表現
            DrawCircle(x, y, 40, color, TRUE);
        }

        // -----------------------------------------------------
        // 追加：フェード方向に応じて「覆う」または「消す」効果を補強
        // -----------------------------------------------------
        if (direction == FadeDirection::Out) {
            // フェードアウト：最終的に完全に覆う
            int fullAlpha = static_cast<int>(t * 255);
            if (fullAlpha > 0) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, fullAlpha);
                DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, color, TRUE);
            }
        }
        else {
            // フェードイン：最初に全画面覆って、中央に渦で穴をあけるように
            int remainAlpha = static_cast<int>((1.0f - t) * 255);
            if (remainAlpha > 0) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, remainAlpha);
                DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, color, TRUE);
            }
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};

#endif // !_SPIRAL_FADE_H_