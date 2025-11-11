/*
 *  @file   TileFade.h
 *  @author Seki
 */

#ifndef _TILE_FADE_H_
#define _TILE_FADE_H_

#include "FadeBase.h"
#include "../GameConst.h"

class TileFade : public FadeBase {
private:
    int color;
    int rows;
    int cols;

public:
    TileFade(float duration, FadeDirection dir, FadeMode mode,
             int color = GetColor(0, 0, 0), int rows = 10, int cols = 16)
        : FadeBase(duration, dir, mode), color(color), rows(rows), cols(cols) {
    }

    void Render() override {
        float t = elapsed / duration;
        t = std::clamp(t, 0.0f, 1.0f);

        const int w = GameConst::WINDOW_WIDTH;
        const int h = GameConst::WINDOW_HEIGHT;

        const int tileW = w / cols;
        const int tileH = h / rows;

        const int totalTiles = rows * cols;
        int activeTiles = static_cast<int>(totalTiles * t + 0.999f);

        if (direction == FadeDirection::Out) {
            // フェードアウト：左上から順に貼る
            for (int i = 0; i < activeTiles; ++i) {
                int row = i / cols;
                int col = i % cols;
                int x0 = col * tileW;
                int y0 = row * tileH;
                DrawBox(x0, y0, x0 + tileW, y0 + tileH, color, TRUE);
            }
        }
        else {
            // フェードイン：右下から順に除く
            int tilesToRemove = activeTiles;
            for (int i = 0; i < totalTiles; ++i) {
                int row = i / cols;
                int col = i % cols;

                // 右下起点に変換
                int x0 = (cols - 1 - (i % cols)) * tileW;   // 右から
                int y0 = (rows - 1 - (i / cols)) * tileH;   // 下から

                if (i < tilesToRemove) {
                    // 除く＝何もしない（透明化）
                }
                else {
                    DrawBox(x0, y0, x0 + tileW, y0 + tileH, color, TRUE);
                }
            }
        }
    }
};

#endif // _TILE_FADE_H_