/*
 *  @file   RenderComponent.h
 *  @author Seki
 */

#ifndef _RENDERCOMPONENT_H_
#define _RENDERCOMPONENT_H_

#include "Component.h"
#include <DxLib.h>

// 前方宣言
class GameObject;

/*
 *  モデル・2D描画コンポーネント
 */
class RenderComponent : public Component {
public:
    virtual ~RenderComponent() = default;
    /*
     *  3D描画処理
     */
    virtual void Render3D() {}
    /*
     *  2D描画処理
     */
    virtual void Render2D() {}

};
#endif // !_RENDERCOMPONENT_H_

