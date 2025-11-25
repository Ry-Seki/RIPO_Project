/*
 *  @file   Collision.h
 *  @author Riku
 */

#include "Collision.h"

/*
 *  AABB同士の交差判定
 *  @param AABB a   判定対象1
 *  @param AABB b   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration) {
    // aの左側面がbの右側面より右にある場合は交差していない
    const float dx0 = b.max.x - a.min.x;
    if (dx0 <= 0)
        return false;
    // aの右側面がbの左側面より左にある場合は交差していない
    const float dx1 = a.max.x - b.min.x;
    if (dx1 <= 0)
        return false;
    // aの下面がbの上面より上にある場合は交差していない
    const float dy0 = b.max.y - a.min.y;
    if (dy0 <= 0)
        return false;
    // aの上面がbの下面より下にある場合は交差していない
    const float dy1 = a.max.y - b.min.y;
    if (dy1 <= 0)
        return false;
    // aの奥側面がbの手前側面より手前にある場合は交差していない
    const float dz0 = b.max.z - a.min.z;
    if (dz0 <= 0)
        return false;
    // aの手前側面がbの奥側面より奥にある場合は交差していない
    const float dz1 = a.max.z - b.min.z;
    if (dz1 <= 0)
        return false;
    
    // XY軸で交差している距離が最も短い軸を計算
    Vector3 length = { dx1, dy1, dz1 }; // 貫通距離の絶対値
    Vector3 signedLength = length;      // 符号付き
    if (dx0 < dx1) {
        length.x = dx0;
        signedLength.x = -dx0;
    }
    if (dy0 < dy1) {
        length.y = dy0;
        signedLength.y = -dy0;
    }
    if (dz0 < dz1) {
        length.z = dz0;
        signedLength.z = -dz0;
    }

    // 最も貫通距離が短い軸を保存
    if (length.x < length.y) {
        if (length.x < length.z) {
            penetration = { signedLength.x, 0, 0 };
            return true;
        }
    }
    else if (length.y < length.z) {
        penetration = { 0, signedLength.y, 0 };
        return true;
    }
    penetration = { 0, 0, signedLength.z };
    return true;
}

/*
 *  カプセル同士の交差判定
 *  @param Capsule a   判定対象1
 *  @param Capsule b   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */
bool Intersect(const Capsule& a, const Capsule& b, Vector3& penetration) {
    




    return false;
}

/*
 *  線分から線分への最近接点
 */
float SegmentBetweenMinLength(const Capsule& a, const Capsule& b) {
    // 各線分の方向ベクトル(非正規化)
    Vector3 aDir = a.endPoint - a.startPoint;
    Vector3 bDir = b.endPoint - b.startPoint;
    // 開始点の差ベクトル
    Vector3 startDiff = a.startPoint - b.startPoint;

    // 各ベクトルの長さの2乗
    float aLengthSq = Dot(aDir, aDir);
    float bLengthSq = Dot(bDir, bDir);
    // 2線分のベクトルの内積
    float segDirDot = Dot(aDir, bDir);
    // 差ベクトルが各線分方向へどれだけ進んだか(差ベクトルを線分に投影)
    float aProjection = Dot(aDir, startDiff);
    float bProjection = Dot(bDir, startDiff);
    
    // 2つの線分が平行か調べる
    // 2つのベクトルの外積の長さ2乗(0に近いほど平行)
    float crossLengthSq = aLengthSq * bLengthSq - segDirDot * segDirDot;
    // 各線分上の最近接点(割合)
    float aSegRatio = 0.0f;
    float bSegRatio = 0.0f;
    // 平行ではないなら
    if (crossLengthSq > EPSILON_SEGMENT) {
        // 最近接点の無制約解(無限の直線として扱った場合の解)
        aSegRatio = (segDirDot * bProjection - bLengthSq * aProjection) / crossLengthSq;
        bSegRatio = (aLengthSq * bProjection - segDirDot * aProjection) / crossLengthSq;
        // 線分上に制限
        aSegRatio = Clamp(aSegRatio, 0, 1);
        bSegRatio = Clamp(bSegRatio, 0, 1);
    }
    // 限りなく平行に近いなら
    else {
        // 線分aの最近接点を開始点とする
        aSegRatio = 0.0f;
        // 線分bが点じゃないなら線分aの開始点を線分bに投影
        if (bLengthSq > EPSILON_SEGMENT) {
            bSegRatio = bProjection / bLengthSq;
        }
        // 線分bが限りなく点に近いなら
        else {
            bSegRatio = 0.0f;
        }
        // 線分上に制限
        bSegRatio = Clamp(bSegRatio, 0, 1);
    }

    // 無制限解を評価
    Vector3 bestPointA = Lerp(a.startPoint, a.endPoint, aSegRatio);
    Vector3 bestPointB = Lerp(b.startPoint, b.endPoint, bSegRatio);

    // 端点チェック
    CheckEndpointClosest(a.startPoint, b.startPoint, b.endPoint, bestPointA, bestPointB, aSegRatio, bSegRatio);
    CheckEndpointClosest(a.endPoint, b.startPoint, b.endPoint, bestPointA, bestPointB, aSegRatio, bSegRatio);
    CheckEndpointClosest(b.startPoint, a.startPoint, a.endPoint, bestPointA, bestPointB, aSegRatio, bSegRatio);
    CheckEndpointClosest(b.endPoint, a.startPoint, a.endPoint, bestPointA, bestPointB, aSegRatio, bSegRatio);

    return 0;
}

void CheckEndpointClosest(
    const Vector3& endpoint,
    const Vector3& segStart,
    const Vector3& segEnd,
    Vector3& bestPointA,
    Vector3& bestPointB,
    float& aSegRatio,
    float& bSegRatio) {
    float t;
    Vector3 closest = PointToSegmentMinLength(segStart, segEnd, endpoint, t);
    Vector3 dist = endpoint - closest;
    float dist2 = Dot(dist, dist);

    Vector3 bestPoint = bestPointA - bestPointB;
    if (dist2 < Dot(bestPoint, bestPoint)) {
        bestPointA = endpoint;
        bestPointB = closest;
        aSegRatio = 0.0f;
        bSegRatio = t;
    }

}

/*
 *  点から線分への最近接点
 *  @param  Vector3 startPos    最近接点を調べる線文の開始点
 *  @param  Vector3 endPos      最近接点を調べる線分の終了点
 *  @param  Vecror3 point       最近接点を調べる点
 *  @param  float   minPoint    線分内の最近接点(0～1)
 *  @return Vector3             最近接点の座標
 */
Vector3 PointToSegmentMinLength(const Vector3& startPos, const Vector3& endPos, const Vector3& point, float& minPoint) {
    // 線分の方向ベクトル(非正規化)
    Vector3 segDir = endPos - startPos;
    // ベクトルの長さの2乗
    float lengthSquare = Dot(segDir, segDir);
    // 長さの2乗が限りなく0に近いならそれを線分ではなく点とする
    if (lengthSquare <= EPSILON_SEGMENT) {
        minPoint = 0.0f;
        return startPos;
    }
    // 点が線分方向にどれだけ進んだか(点を線分に投影)
    float projection = Dot(point - startPos, segDir);
    // 線分上の割合に変換
    float segmentRatio = projection / lengthSquare;
    // 線分上に制限
    segmentRatio = Clamp(segmentRatio, 0, 1);

    minPoint = segmentRatio;
    return Lerp(startPos, endPos, segmentRatio);
}