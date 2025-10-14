/*
 *  @file   SpriteRenderer.h
 *  @author Seki
 */

#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER__

#include "RenderComponent.h"

/*
 *  2Dの描画コンポーネント
 */
class SpriteRenderer : public RenderComponent {
private:
    int spriteHandle = -1;  // 画像ハンドル
    int x = 0, y = 0;   // 画像の座標

public:
    /*
     *  2Dの描画
     */
    void Render2D() override {
        if (spriteHandle != -1) DrawGraph(x, y, spriteHandle, TRUE);
    }
    /*
     *  破棄処理
     */
    void OnDestroy() override {
        if (spriteHandle != -1) {
            DeleteGraph(spriteHandle);
            spriteHandle = -1;
        }
    }

public:
    /*
     *  画像ハンドル設定
     *  param[in]           int& setHandle  画像ハンドル
     */
    inline void SetImage(int& setHandle) { spriteHandle = setHandle; }
    /*
     *  座標の設定
     *  param[in]           int& px     X座標
     *  param[in]           int& py     Y座標
     */
    inline void SetPosition(int& px, int& py) { x = px; y = py; }

};
#endif // !_SPRITERENDERER_H_