/*
 *  @file   ModelRenderer.h
 */

#ifndef _MODELRENDERER_H_
#define _MODELRENDERER_H_

#include "RenderComponent.h"
#include "GameObject.h"

class ModelRenderer : public RenderComponent {
private:
    int modelID = -1;

public:
    void SetModel(int id) { modelID = id; }

    void Render3D() override {
        if (modelID == -1) return;
        auto obj = GetOwner();
        if (!obj) return;
        MV1SetPosition(modelID, VGet(obj->position.x, obj->position.y, obj->position.z));
        MV1SetRotationXYZ(modelID, VGet(obj->rotation.x, obj->rotation.y, obj->rotation.z));
        MV1SetScale(modelID, VGet(obj->scale.x, obj->scale.y, obj->scale.z));
        MV1DrawModel(modelID);
    }

    void OnDestroy() override {
        if (modelID != -1) {
            MV1DeleteModel(modelID);
            modelID = -1;
        }
    }
};
#endif // !_MODELRENDERER_H_

