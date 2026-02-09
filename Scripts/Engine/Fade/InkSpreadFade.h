/*
 *  @file   InkSpreadFade.h
 *  @author Seki
 */

#ifndef _INK_SPREAD_FADE_H_
#define _INK_SPREAD_FADE_H_

#include "FadeBase.h"
#include "../GameConst.h"
#include "../ScreenSize.h"

#include <DxLib.h>
#include <algorithm>
#include <cmath>

/*
 *  ÉCÉìÉNçLÇ™ÇËÉtÉFÅ[Éh
 */
class InkSpreadFade : public FadeBase {
private:
    int color;
    float spreadSpeed;
    float waveAmplitude;
    float waveFrequency;
public:
    InkSpreadFade(float duration, FadeDirection dir, FadeMode mode,
                  int color = GetColor(0, 0, 0),
                  float spreadSpeed = 1.0f,
                  float waveAmplitude = 20.0f,
                  float waveFrequency = 5.0f)
        : FadeBase(duration, dir, mode),
        color(color),
        spreadSpeed(spreadSpeed),
        waveAmplitude(waveAmplitude),
        waveFrequency(waveFrequency) {
    }
    void Render() override {
        float t = std::clamp(elapsed / duration, 0.0f, 1.0f);
        float progress = (direction == FadeDirection::In) ? (1.0f - t) : t;
        auto screen = GetScreenSize();
        int screenW = screen.width;
        int screenH = screen.height;

        int cx = screenW / 2, cy = screenH / 2;

        // ÉAÉãÉtÉ@
        int alpha = static_cast<int>(progress * 255);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // ägéUîºåa
        float maxRadius = std::hypotf((float)cx, (float)cy) * 1.2f;
        float radius = maxRadius * (progress * spreadSpeed);
        radius += waveAmplitude * std::sin(elapsed * waveFrequency);

        // îwåiÇï¢Ç§
        DrawBox(0, 0, screenW, screenH, color, TRUE);

        // åäÇçÏÇÈ
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawCircle(cx, cy, static_cast<int>(radius), GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};

#endif // !_INK_SPREAD_FADE_H_