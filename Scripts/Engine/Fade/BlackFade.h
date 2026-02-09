/*
 *  @file   BlackFade.h
 *  @author Seki
 */

#ifndef _BLACKFADE_H_
#define _BLACKFADE_H_

#include "FadeBase.h"
#include "../ScreenSize.h"

/*
 *  黒フェード
 */
class BlackFade : public FadeBase {
public:
    /*
     *  コンストラクタ
     *  @param[in]  float         duration  フェード時間
     *  @param[in]  FadeDirection dir       フェードアウト・フェードインの選択
     *  @param[in]  FadeMode      mode      フェード中、Sceneの更新を止めるか
     */
    BlackFade(float duration, FadeDirection dir, FadeMode mode)
        : FadeBase(duration, dir, mode) {}

    /*
     *  描画処理
     */
    void Render() override {
        float alpha = 0.0f;
        auto screen = GetScreenSize();

        if (direction == FadeDirection::In)
            alpha = 1.0f - (elapsed / duration); // フェードイン: 透明に
        else
            alpha = elapsed / duration;           // フェードアウト: 黒く

        if (alpha > 1.0f) alpha = 1.0f;
        if (alpha < 0.0f) alpha = 0.0f;
        SetDrawAreaFull();
        // ここが重要
        SetUseZBuffer3D(FALSE);
        SetWriteZBuffer3D(FALSE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha * 255));
        DrawBox(0, 0, screen.width, screen.height, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 必要なら戻す
        SetUseZBuffer3D(TRUE);
        SetWriteZBuffer3D(TRUE);
    }
};

#endif // !_BLACKFADE_H_

