/*
*   @file   RenderComponent.h
*/
#ifndef _RENDERCOMPONENT_H_
#define _RENDERCOMPONENT_H_

#include "Component.h"
#include <DxLib.h>

// ‘O•ûéŒ¾
class GameObject;

class RenderComponent : public Component {
public:
    virtual ~RenderComponent() = default;

    virtual void Render3D() {}
    virtual void Render2D() {}

};
#endif // !_RENDERCOMPONENT_H_

