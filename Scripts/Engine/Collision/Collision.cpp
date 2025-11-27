/*
 *  @file   Collision.h
 *  @author Riku
 */

#include "Collision.h"

 /*
  *  AABB同士の交差判定
  *  @param[in]  AABB a   判定対象1
  *  @param[in]  AABB b   判定対象2
  *  @param[out] Vector3 penetration 貫通ベクトル
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
 *  点から線分への最近接点
 *  @param[in]  Vector3 startPos    最近接点を調べる線文の開始点
 *  @param[in]  Vector3 endPos      最近接点を調べる線分の終了点
 *  @param[in]  Vecror3 point       最近接点を調べる点
 *  @param[out] float   minRatio    線分内の最近接点(0～1)
 *  @return     Vector3             最近接点の座標
 */
Vector3 PointToSegmentMinLength(const Vector3& startPos, const Vector3& endPos, const Vector3& point, float& minRatio) {
	// 線分の方向ベクトル(非正規化)
	Vector3 segDir = endPos - startPos;
	// ベクトルの長さの2乗
	float lengthSquare = Dot(segDir, segDir);
	// 長さの2乗が限りなく0に近いならそれを線分ではなく点とする
	if (lengthSquare <= EPSILON) {
		minRatio = 0.0f;
		return startPos;
	}
	// 点が線分方向にどれだけ進んだか(点を線分に投影)
	float projection = Dot(point - startPos, segDir);
	// 線分上の割合に変換
	float segmentRatio = projection / lengthSquare;
	// 線分上に制限
	segmentRatio = Clamp(segmentRatio, 0, 1);

	minRatio = segmentRatio;
	return Lerp(startPos, endPos, segmentRatio);
}

/*
 *  計算結果が最適解の場合に値を更新する
 *  @param[in]  Vector3 aPointCand      線分aの最近接点候補(座標)
 *  @param[in]  Vector3 bPointCand      線分bの最近接点候補(座標)
 *  @param[in]  float   aMinRatioCand   線分aの最近接点候補(0～1)
 *  @param[in]  float   bMinRatioCand   線分bの最近接点候補(0～1)
 *  @param[out] Vector3 bestPointA      線分aの現状の最近接点(座標)
 *  @param[out] Vector3 bestPointB      線分bの現状の最近接点(座標)
 *  @param[out] float   aSegRatio       線分aの現状の最近接点(0～1)
 *  @param[out] float   bSegRatio       線分bの現状の最近接点(0～1)
 *  @param[out] float   bestLengthSq    現状の最短距離の2乗
 */
void TryUpdateBest(
	const Vector3& aPointCand,
	const Vector3& bPointCand,
	float aMinRatioCand,
	float bMinRatioCand,
	Vector3& bestPointA,
	Vector3& bestPointB,
	float& aSegRatio,
	float& bSegRatio,
	float& bestLengthSq) {
	// 端点から線分への最近接点が現状の最近接点の長さより小さいなら値を更新
	float lengthSquare = Dot(aPointCand - bPointCand, aPointCand - bPointCand);
	if (lengthSquare < bestLengthSq) {
		bestLengthSq = lengthSquare;
		bestPointA = aPointCand;
		bestPointB = bPointCand;
		aSegRatio = aMinRatioCand;
		bSegRatio = bMinRatioCand;
	}
}

/*
 *  線分から線分への最近接点
 *  @param[in] Segment a   判定対象1
 *  @param[in] Segment b   判定対象2
 */
void SegmentBetweenMinLength(const Segment& a, const Segment& b, Vector3& aMinPoint, Vector3& bMinPoint, float& aMinRatio, float& bMinRatio) {
	Vector3 aStart = a.startPoint;
	Vector3 aEnd = a.endPoint;
	Vector3 bStart = b.startPoint;
	Vector3 bEnd = b.endPoint;
	// 各線分の方向ベクトル(非正規化)
	Vector3 aDir = aEnd - aStart;
	Vector3 bDir = bEnd - bStart;
	// 開始点の差ベクトル
	Vector3 startDiff = aStart - bStart;

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
	if (crossLengthSq > EPSILON) {
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
		if (bLengthSq > EPSILON) {
			bSegRatio = bProjection / bLengthSq;
		}
		// 線分bが限りなく点に近いなら
		else {
			bSegRatio = 0.0f;
		}
		// 線分上に制限
		bSegRatio = Clamp(bSegRatio, 0, 1);
	}

	// 無制約解で得られた最近接点を座標に変換
	Vector3 bestPointA = Lerp(aStart, aEnd, aSegRatio);
	Vector3 bestPointB = Lerp(bStart, bEnd, bSegRatio);

	// 各線分の端点から相手線分上への最近接点を求め、最小となるものを採用する

	// 最近接点間の長さの2乗
	float bestLengthSq = Dot(bestPointA - bestPointB, bestPointA - bestPointB);
	// 各線分の最近接点座標候補
	Vector3 aPointCand, bPointCand;
	// 各線分上の最近接点候補
	float aMinRatioCand, bMinRatioCand;

	// aの開始点からbの線分
	aMinRatioCand = 0.0f;
	aPointCand = aStart;
	bPointCand = PointToSegmentMinLength(bStart, bEnd, aStart, bMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// aの終了点からbの線分
	aMinRatioCand = 1.0f;
	aPointCand = aEnd;
	bPointCand = PointToSegmentMinLength(bStart, bEnd, aEnd, bMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// bの開始点からaの線分
	bMinRatioCand = 0.0f;
	bPointCand = bStart;
	aPointCand = PointToSegmentMinLength(aStart, aEnd, bStart, aMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// bの終了点からaの線分
	bMinRatioCand = 1.0f;
	bPointCand = bEnd;
	aPointCand = PointToSegmentMinLength(aStart, aEnd, bEnd, aMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// 最終的な最近接点を返す
	aMinPoint = bestPointA;
	bMinPoint = bestPointB;
	aMinRatio = aSegRatio;
	bMinRatio = bSegRatio;
}

/*
 *  カプセル同士の交差判定
 *  @param[in]  Capsule a   判定対象1
 *  @param[in]  Capsule b   判定対象2
 *  @param[out] Vector3 penetration 貫通ベクトル
 */
bool Intersect(const Capsule& a, const Capsule& b, Vector3& penetration) {
	Vector3 aMinPoint, bMinPoint;
	float aMinRatio, bMinRatio;
	// 各カプセルの線分同士の最近接点を求める
	SegmentBetweenMinLength(a.segment, b.segment, aMinPoint, bMinPoint, aMinRatio, bMinRatio);
	// 2つの最近接点の差ベクトル
	Vector3 difference = aMinPoint - bMinPoint;
	// 最近接点間の長さの2乗(比較の時点では2乗で十分なので今はsqrtしない)
	float lengthSquare = Dot(difference, difference);
	// 各カプセルの半径の合計とその2乗
	float sumRadius = a.radius + b.radius;
	float sumRadiusSq = sumRadius * sumRadius;
	
	// 最近接点間の長さが半径の合計以上な時点で衝突はしていない
	if (lengthSquare >= sumRadiusSq) return false;

	// 貫通距離を出す
	// 最近接点間の長さが必要になるので2乗を直す
	float length = std::sqrt(lengthSquare);
	// 貫通方向ベクトル
	Vector3 direction;
	// 最近接点間の長さが0に限りなく近くなければ正規化
	if (length > EPSILON) {
		direction = difference / length;
	}
	// 最近接点間の長さが限りなく0に近い場合は最も成分の大きい軸に固定
	else {
		if (fabs(difference.x) > fabs(difference.y)) {
			if (fabs(difference.x) > fabs(difference.z)) {
				direction = V_RIGHT;
			}
			else {
				direction = V_FORWARD;
			}
		}
		else {
			if (fabs(difference.y) > fabs(difference.z)) {
				direction = V_UP;
			}
			else {
				direction = V_FORWARD;
			}
		}
	}
	// 貫通深度
	float depth = sumRadius - length;
	// 貫通距離
	penetration = direction * depth;
	return true;
}
