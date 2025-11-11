/*
 *  @file   FadeFactory.h
 *  @author Seki
 */

#ifndef _FADE_FACTORY_H_
#define _FADE_FACTORY_H_

#include "FadeBase.h"
#include "BlackFade.h"
#include "SpiralFade.h"
#include "TileFade.h"


enum class FadeType {
    Black,
    Spiral,
    Tile,
};

/*
 *  各フェードの生成するファクトリクラス
 */
class FadeFactory {
public:
    /*
     *  フェード生成処理
     */
    static FadeBasePtr CreateFade(FadeType type, float duration,
                                  FadeDirection dir, FadeMode mode) {
        switch (type) {
            case FadeType::Black:
                return std::make_shared<BlackFade>(duration, dir, mode);
            case FadeType::Spiral:
                return std::make_shared<SpiralFade>(duration, dir, mode);
            case FadeType::Tile:
                return std::make_shared<TileFade>(duration, dir, mode);
        }
        return nullptr;
    }
};

#endif // !_FADE_FACTORY_H_

