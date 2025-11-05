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
    int modelHandle = -1;

public:
    /*
     *  描画処理
     */
    void Render3D() override {
        if (modelHandle == -1) return;
        auto obj = GetOwner();
        if (!obj) return;
        MV1SetPosition(modelHandle, VGet(obj->position.x, obj->position.y, obj->position.z));
        MV1SetRotationXYZ(modelHandle, VGet(obj->rotation.x, obj->rotation.y, obj->rotation.z));
        MV1SetScale(modelHandle, VGet(obj->scale.x, obj->scale.y, obj->scale.z));
        MV1DrawModel(modelHandle);
    }
    /*
     *  破棄処理
     */
    void OnDestroy() override {
        if (modelHandle != -1) {
            MV1DeleteModel(modelHandle);
            modelHandle = -1;
        }
    }

public:
    /*
     *  モデルハンドルの設定
     *  param[in]   const int setHandle   モデルハンドル
     */
    void SetModelHandle(const int setHandle) { modelHandle = MV1DuplicateModel(setHandle); }
};
#endif // !_MODELRENDERER_H_

