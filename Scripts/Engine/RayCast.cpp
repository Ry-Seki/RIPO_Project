/*
 *	@file	RayCast.cpp
 *	@author	Riku
 */
#include "RayCast.h"


 /*
  *  レイキャスト
  *  @param Engine* engine  
  *  @param Vector3 startPosition   レイ開始位置
  *  @param Vector3 direction       レイの方向
  *  @param float   hitLength       レイが当たるまでの距離
  */
bool RayCast(Engine* engine, Vector3 startPosition, Vector3 direction, float& hitLength) {
    ScenePtr scene = engine->GetCurrentScene();
    // 全てのオブジェクトでコライダーの座標を計算
    std::vector<Scene::WorldColliderList> colliders = scene->ChangeGameObjectWorldColliders();
}

 /*
  *  RayとAABBの交差判定
  *  @param Ray  ray   判定対象1
  *  @param AABB box   判定対象2
  *  @param float hitLength 当たるまでの距離
  */
bool RayIntersect(const Ray& ray, const AABB& box, float& hitLength) {
    // rayのdirが0なら0のまま
    float dx0 = 0;
    float dx1 = 0;
    float dy0 = 0;
    float dy1 = 0;
    float dz0 = 0;
    float dz1 = 0;
    // 0除算回避(x軸判定)
    if (ray.dir.x == 0) {
        // rayがx軸上でboxの左側に当たるまでの距離が0以下かつ開始位置がboxの左側より左にある場合は当たらない
        dx0 = (box.min.x - ray.start.x) / ray.dir.x;
        if (dx0 <= 0 && box.min.x >= ray.start.x)
            return false;
        // rayがx軸上でboxの右側に当たるまでの距離が0以下かつ開始位置がboxの右側より右にある場合は当たらない
        dx1 = (box.max.x - ray.start.x) / ray.dir.x;
        if (dx1 <= 0 && box.max.x <= ray.start.x)
            return false;
    }
    else {
        // rayがboxの左側より左にある場合は当たらない
        if (box.min.x >= ray.start.x)
            return false;
        // rayがboxの右側より右にある場合は当たらない
        if (box.max.x <= ray.start.x)
            return false;
    }

    // 0除算回避(y軸判定)
    if (ray.dir.y != 0) {
        // rayがy軸上でboxの下に当たるまでの距離が0以下かつ開始位置がboxの下側より下にある場合は当たらない
        dy0 = (box.min.y - ray.start.y) / ray.dir.y;
        if (dy0 <= 0 && box.min.y >= ray.start.y)
            return false;
        // rayがy軸上でboxの上側に当たるまでの距離が0以下かつ開始位置がboxの上側より上にある場合は当たらない
        dy1 = (box.max.y - ray.start.y) / ray.dir.y;
        if (dy1 <= 0 && box.max.y <= ray.start.y)
            return false;
    }
    else {
        // rayがboxの下側より下にある場合は当たらない
        if (box.min.y >= ray.start.y)
            return false;
        // rayがboxの上側より上にある場合は当たらない
        if (box.max.y <= ray.start.y)
            return false;
    }

    // 0除算回避(z軸判定)
    if (ray.dir.z != 0) {
        // rayがz軸上でboxの奥側に当たるまでの距離が0以下かつ開始位置がboxの奥側より奥にある場合は当たらない
        dz0 = (box.min.z - ray.start.z) / ray.dir.z;
        if (dz0 <= 0 && box.min.z >= ray.start.z)
            return false;
        // rayがz軸上でboxの手前側に当たるまでの距離が0以下かつ開始位置がboxの手前側より手前にある場合は当たらない
        dz1 = (box.max.z - ray.start.z) / ray.dir.z;
        if (dz1 <= 0 && box.max.z <= ray.start.z)
            return false;
    }
    else {
        // rayがboxの奥側より奥にある場合は当たらない
        if (box.min.z >= ray.start.z)
            return false;
        // rayがboxの手前側より手前にある場合は当たらない
        if (box.max.z <= ray.start.z)
            return false;
    }

    // 各軸の当たるまでの距離が最も短い長さを計算
    Vector3 length = { dx1, dy1, dz1 };
    if (dx0 < dx1) {
        length.x = dx0;
    }
    if (dy0 < dy1) {
        length.y = dy0;
    }
    if (dz0 < dz1) {
        length.z = dz0;
    }
    
    // 全ての軸で衝突するまでの長さを保存
    if (length.x > length.y) {
        if (length.x > length.z) {
            hitLength = length.x;
            return true;
        }
    }
    else if (length.y > length.z) {
        hitLength = length.y;
        return true;
    }
    hitLength = length.z;
    return true;
}