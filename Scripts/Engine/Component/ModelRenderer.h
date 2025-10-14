/*
 *  @file   ModelRenderer.h
 *  @author Seki
 */

#ifndef _MODELRENDERER_H_
#define _MODELRENDERER_H_

#include "RenderComponent.h"
#include "../GameObject.h"

/*
 *  3Dモデルコンポーネント
 */
class ModelRenderer : public RenderComponent {
private:
    int modelID = -1;

public:
    /*
     *  描画処理
     */
    void Render3D() override {
        if (modelID == -1) return;
        auto obj = GetOwner();
        if (!obj) return;
        MV1SetPosition(modelID, VGet(obj->position.x, obj->position.y, obj->position.z));
        MV1SetRotationXYZ(modelID, VGet(obj->rotation.x, obj->rotation.y, obj->rotation.z));
        MV1SetScale(modelID, VGet(obj->scale.x, obj->scale.y, obj->scale.z));
        MV1DrawModel(modelID);
    }
    /*
     *  破棄処理
     */
    void OnDestroy() override {
        if (modelID != -1) {
            MV1DeleteModel(modelID);
            modelID = -1;
        }
    }

public:
    /*
     *  モデルハンドルの設定
     *  param[in]           int& setValue   モデルハンドル
     */
    void SetModel(int& setValue) { modelID = setValue; }
};
#endif // !_MODELRENDERER_H_

