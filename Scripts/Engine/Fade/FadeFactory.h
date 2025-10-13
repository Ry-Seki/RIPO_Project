/*
 *  @file   FadeFactory.h
 *  @author Seki
 */

#ifndef _FADE_FACTORY_H_
#define _FADE_FACTORY_H_

#include "FadeBase.h"
#include "BlackFade.h"
#include <memory>

enum class FadeType {
    Black
};
/*
 *  フェード生成処理
 */
inline FadeBasePtr CreateFade(FadeType type, float duration,
                                            FadeDirection dir, FadeMode mode) {
    switch (type) {
        case FadeType::Black:
            return std::make_shared<BlackFade>(duration, dir, mode);
    }
    return nullptr;
}
#endif // !_FADE_FACTORY_H_

