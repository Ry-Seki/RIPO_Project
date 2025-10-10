#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER__
#endif // !_SPRITERENDERER_H_

#include "RenderComponent.h"

class SpriteRenderer : public RenderComponent {
private:
    int handle = -1;
    int x = 0, y = 0;

public:
    void SetImage(int img) { handle = img; }
    void SetPosition(int px, int py) { x = px; y = py; }

    void Render2D() override {
        if (handle != -1) DrawGraph(x, y, handle, TRUE);
    }

    void OnDestroy() override {
        if (handle != -1) {
            DeleteGraph(handle);
            handle = -1;
        }
    }
};