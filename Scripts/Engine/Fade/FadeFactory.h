#ifndef _FADEFACTORY_H_
#define _FADEFACTORY_H_

#include "FadeBase.h"
#include "BlackFade.h"
#include <memory>

enum class FadeType {
    Black
};

inline FadeBasePtr CreateFade(FadeType type, float duration,
                                            FadeDirection dir, FadeMode mode) {
    switch (type) {
        case FadeType::Black:
            return std::make_shared<BlackFade>(duration, dir, mode);
    }
    return nullptr;
}
#endif // !_FADEFACTORY_H_

