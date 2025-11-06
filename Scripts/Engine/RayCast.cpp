/*
 *	@file	RayCast.cpp
 *	@author	Riku
 */
#include "RayCast.h"


/*
 *  レイキャスト
 *  @param  Engine*         engine          
 *  @param  Vector3         startPosition   レイ開始位置
 *  @param  Vector3         direction       レイの方向
 *  @param  float&          hitLength       レイが当たるまでの距離
 *  @param  GameObject*&    hitObject       当たったオブジェクト
 */
bool RayCast(Engine* engine, Vector3 startPosition, Vector3 direction, float& hitLength, GameObject*& hitObject) {
    ScenePtr scene = engine->GetCurrentScene();
    Ray ray = { startPosition, direction.Normalized()};
    bool hit = false;
    // 全てのオブジェクトでコライダーの座標を計算
    std::vector<Scene::WorldColliderList> colliders = scene->ChangeGameObjectWorldColliders();

    // ゲームオブジェクト毎の衝突判定
    for (auto box = colliders.begin(); box != colliders.end(); box++) {
        GameObject* objBox = box->at(0).origin->GetOwner();
        // 削除済みは処理しない
        if (objBox->IsDestroyed())
            continue;

        // 衝突判定
        for (const auto& boxCol : *box) {
            if (RayIntersect(ray, boxCol.world, hitLength, hitObject, objBox, hit))
                hit = true;
        }
    }
    return hit;
}

/*
 *  RayとAABBの交差判定
 *  @param  Ray&            ray         判定対象1
 *  @param  AABB&           box         判定対象2
 *  @param  float&          hitLength   当たるまでの距離
 *  @param  GameObject*&    hitObject   当たった最も近いオブジェクト
 *  @param  GameObject*     checkObject 判定チェックオブジェクト
 *  @param  bool            isHit       当たったかどうか
 */
bool RayIntersect(const Ray& ray, const AABB& box, float& hitLength, GameObject*& hitObject, GameObject* checkObject, bool isHit) {
    // rayのdirが0なら0のまま
    float dxMin = 0;
    float dxMax = 0;
    float dyMin = 0;
    float dyMax = 0;
    float dzMin = 0;
    float dzMax = 0;
    // x軸の交差判定
    if (!IntersectCheck(box.min.x, box.max.x, ray.start.x, ray.dir.x, dxMin, dxMax)) return false;
    // y軸の交差判定
    if (!IntersectCheck(box.min.y, box.max.y, ray.start.y, ray.dir.y, dyMin, dyMax)) return false;
    // z軸の交差判定
    if (!IntersectCheck(box.min.z, box.max.z, ray.start.z, ray.dir.z, dzMin, dzMax)) return false;

    // 各軸の当たるまでの距離が最も短い長さを計算
    Vector3 length = { dxMax, dyMax, dzMax };
    if (dxMin < dxMax) {
        length.x = dxMin;
    }
    if (dyMin < dyMax) {
        length.y = dyMin;
    }
    if (dzMin < dzMax) {
        length.z = dzMin;
    }
    
    // 全ての軸で衝突するまでの長さを保存
    float allHitLength = 0;
    if (length.x > length.y) {
        if (length.x > length.z) {
            allHitLength = length.x;
        }
    }
    else if (length.y > length.z) {
        allHitLength = length.y;
    }
    else allHitLength = length.z;

    // より距離の短い方を保存
    if (hitLength > allHitLength) {
        hitLength = allHitLength;
        hitObject = checkObject;
    }

    // 初めてのhitなら当たった距離とオブジェクトを保存
    if (!isHit) {
        hitLength = allHitLength;
        hitObject = checkObject;
    }
    return true;
}

/*
 *  各軸の交差判定
 *  @param  float   boxMin  AABBの各軸の最小位置
 *  @param  float   boxMax  AABBの各軸の最大位置
 *  @param  float   rayPos  レイの各軸の開始位置
 *  @param  float   rayDir  レイの各軸の方向ベクトル
 *  @param  float&  dMin    最初に当たるまでの距離
 *  @param  float&  dMax    最後に当たるまでの距離
 */
bool IntersectCheck(float boxMin, float boxMax, float rayPos, float rayDir, float& dMin, float& dMax) {
    // 0除算回避
    if (rayDir != 0) {
        // rayが各軸上でboxに最初に当たるまでの距離が0以下かつ開始位置がboxの内側より外にある場合は当たらない
        dMin = (boxMin - rayPos) / rayDir;
        if (dMin <= 0 && boxMin >= rayPos)
            return false;
        // rayが各軸上でboxに最後に当たるまでの距離が0以下かつ開始位置がboxの内側より外にある場合は当たらない
        dMax = (boxMax - rayPos) / rayDir;
        if (dMax <= 0 && boxMax <= rayPos)
            return false;
    }
    else {
        // rayがboxの内側より外にある場合は当たらない
        if (boxMin >= rayPos)
            return false;
        // rayがboxの内側より外にある場合は当たらない
        if (boxMax <= rayPos)
            return false;
    }
    return true;
}
