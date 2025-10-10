#ifndef _BLACKFADE_H_
#define _BLACKFADE_H_

#include "FadeBase.h"

class BlackFade : public FadeBase {
public:
    BlackFade(float duration, FadeDirection dir, FadeMode mode)
        : FadeBase(duration, dir, mode) {}

    void Render() override {
        float alpha = 0.0f;
        if (direction == FadeDirection::In)
            alpha = 1.0f - (elapsed / duration); // フェードイン: 透明に
        else
            alpha = elapsed / duration;           // フェードアウト: 黒く

        if (alpha > 1.0f) alpha = 1.0f;
        if (alpha < 0.0f) alpha = 0.0f;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha * 255));
        DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};

#endif // !_BLACKFADE_H_

