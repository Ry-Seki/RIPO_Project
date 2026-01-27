/*
 *  @file   Collision.cpp
 *  @author Riku
 */

#include "Collision.h"
#include <cfloat>

 /*
  *  点から線分への最近接点
  *  @param[in]  Vecror3 point       最近接点を調べる点
  *  @param[in]  Segment segment		最近接点を調べる線分
  *  @param[out] float   minRatio    線分上の最近接点(0～1)
  *  @return     Vector3             最近接点の座標
  */
Vector3 PointToSegmentMinLength(const Vector3& point, const Segment& segment, float& minRatio) {
	Vector3 startPos = segment.startPoint;
	Vector3 endPos = segment.endPoint;
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
 *	点からAABBへの最近接点
 *	@param[in]	Vector3	point	最近接点を調べる点
 *	@param[in]	AABB	aabb	最近接点を調べるAABB
 *  @return		Vector3			最近接点の座標
 */
Vector3 PointToAABBMinLength(const Vector3& point, const AABB& box) {
	return {
		Clamp(point.x, box.min.x, box.max.x),
		Clamp(point.y, box.min.y, box.max.y),
		Clamp(point.z, box.min.z, box.max.z)
	};
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
 *  @param[in]	Segment a			最近接点を調べる線分1
 *  @param[in]	Segment b			最近接点を調べる線分2
 *	@param[out] Vector3	aMinPoint	線分1の最近接点座標
 *	@param[out] Vector3	aMinPoint	線分2の最近接点座標
 *	@param[out] float	aMinRatio	線分1上の最近接点(0～1)
 *	@param[out] float	aMinRatio	線分2上の最近接点(0～1)
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
	bPointCand = PointToSegmentMinLength(aStart, b, bMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// aの終了点からbの線分
	aMinRatioCand = 1.0f;
	aPointCand = aEnd;
	bPointCand = PointToSegmentMinLength(aEnd, b, bMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// bの開始点からaの線分
	bMinRatioCand = 0.0f;
	bPointCand = bStart;
	aPointCand = PointToSegmentMinLength(bStart, a, aMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// bの終了点からaの線分
	bMinRatioCand = 1.0f;
	bPointCand = bEnd;
	aPointCand = PointToSegmentMinLength(bEnd, a, aMinRatioCand);
	TryUpdateBest(aPointCand, bPointCand, aMinRatioCand, bMinRatioCand, bestPointA, bestPointB, aSegRatio, bSegRatio, bestLengthSq);

	// 最終的な最近接点を返す
	aMinPoint = bestPointA;
	bMinPoint = bestPointB;
	aMinRatio = aSegRatio;
	bMinRatio = bSegRatio;
}

/*
 *	1つの軸のスラブ判定
 *	@param[in]	float	start		軸単位の線の開始点
 *	@param[in]	float	direction	線の向く方向
 *	@param[in]	float	boxMin		軸単位のAABBの最小点
 *	@param[in]	float	boxMax		軸単位のAABBの最大点
 *	@param[out]	float	segMinRatio 線分の有効区間の開始点
 *	@param[out]	float	segMaxRatio	線分の有効区間の終了点
 *  @return		bool				交差するかどうか
 */
bool IntersectSlab(float start, float direction, float boxMin, float boxMax, float& segMinRatio, float& segMaxRatio) {
	// 限りなく平行に近いなら
	if (fabs(direction) < EPSILON) {
		// 線分の開始点がその軸上でboxの中になければ交差はしていない
		return (start >= boxMin && start <= boxMax);
	}

	// dirの逆数(除算をできるだけ避けるため)
	float reciprocalDir = 1.0f / direction;
	// スラブに到達する地点
	float enterRatio = (boxMin - start) * reciprocalDir;
	float exitRatio = (boxMax - start) * reciprocalDir;

	// enterRatio <= exitRatio にする(方向によっては逆転する可能性があるため)
	if (enterRatio > exitRatio)
		std::swap(enterRatio, exitRatio);

	// 線分の有効区間を更新
	if (enterRatio > segMinRatio)
		segMinRatio = enterRatio;
	if (exitRatio < segMaxRatio)
		segMaxRatio = exitRatio;

	// 交差区間がなくなったら交差していない
	return segMinRatio <= segMaxRatio;
}

/*
 *  線分からAABBへの最近接点
 *  @param[in]	Segment segment		最近接点を調べる線分
 *  @param[in]	AABB	box			最近接点を調べるAABB
 *	@param[out] Vector3	segMinPoint	線分の最近接点座標
 *	@param[out]	Vector3	boxMinPoint	AABBの最近接点座標
 *  @param[out]	float	minLengthSq	最近接点間の長さ2乗
 */
void SegmentToAABBMinLength(const Segment& segment, const AABB& box, Vector3& segMinPoint, Vector3& boxMinPoint, float& minLengthSq) {
	Vector3 segStart = segment.startPoint;
	Vector3 segEnd = segment.endPoint;
	Vector3 boxMin = box.min;
	Vector3 boxMax = box.max;
	// 線分の方向ベクトル(非正規化)
	Vector3 segDir = segEnd - segStart;
	// より小さい値を探していくため初期値は最大
	minLengthSq = MAX_DISTANCE;

	// 線分がAABB内に入っているか

	// 線分がAABBと交差しているか
	auto IntersectSegmentAABB = [&](Vector3& intersectPoint) {
		// 線分の有効区間
		float segMinRatio = 0.0f;
		float segMaxRatio = 1.0f;

		// 各軸のスラブ判定
		// xスラブ
		if (!IntersectSlab(segStart.x, segDir.x, boxMin.x, boxMax.x, segMinRatio, segMaxRatio))
			return false;
		// yスラブ
		if (!IntersectSlab(segStart.y, segDir.y, boxMin.y, boxMax.y, segMinRatio, segMaxRatio))
			return false;
		// zスラブ
		if (!IntersectSlab(segStart.z, segDir.z, boxMin.z, boxMax.z, segMinRatio, segMaxRatio))
			return false;

		// 全ての軸で交差しているなら線分とAABBは交差している
		intersectPoint = segStart + segDir * segMinRatio;
		return true;
	};

	// 交差しているなら最近接点は0
	Vector3 intersect;
	if (IntersectSegmentAABB(intersect)) {
		segMinPoint = intersect;
		boxMinPoint = intersect;
		minLengthSq = 0;
		return;
	}

	// 線分の端点からAABBへの最近接点

	// 現状の最短より短ければ値更新
	auto CheckCandidate = [&](const Vector3& segPoint, const Vector3& boxPoint) {
		float lengthSq = Dot((segPoint - boxPoint), (segPoint - boxPoint));
		if (lengthSq < minLengthSq) {
			segMinPoint = segPoint;
			boxMinPoint = boxPoint;
			minLengthSq = lengthSq;
		}
	};

	// 線分の開始点からAABB
	Vector3 startMinPoint = PointToAABBMinLength(segStart, box);
	CheckCandidate(segStart, startMinPoint);

	// 線分の終了点からAABB
	Vector3 engMinPoint = PointToAABBMinLength(segEnd, box);
	CheckCandidate(segEnd, engMinPoint);

	// AABBの8コーナーから線分への最近接点

	// 各コーナーから線分への最近接点
	auto CheckCorner = [&](const float x, const float y, const float z) {
		Vector3 corner = { x, y, z };
		float minRatio;
		Vector3 segPoint = PointToSegmentMinLength(corner, segment, minRatio);
		CheckCandidate(segPoint, corner);
	};

	// 8つのコーナーで最近接点を求める
	CheckCorner(boxMin.x, boxMin.y, boxMin.z);
	CheckCorner(boxMax.x, boxMin.y, boxMin.z);
	CheckCorner(boxMin.x, boxMax.y, boxMin.z);
	CheckCorner(boxMax.x, boxMax.y, boxMin.z);
	CheckCorner(boxMin.x, boxMin.y, boxMax.z);
	CheckCorner(boxMax.x, boxMin.y, boxMax.z);
	CheckCorner(boxMin.x, boxMax.y, boxMax.z);
	CheckCorner(boxMax.x, boxMax.y, boxMax.z);

	// AABBの12辺から線分への最近接点

	// 各辺から線分への最近接点
	auto CheckEdge = [&](Vector3 start, Vector3 end) {
		Segment edgeSeg(start, end);
		Vector3 aMinPoint, bMinPoint;
		float aMinRatio, bMinRatio;
		SegmentBetweenMinLength(segment, edgeSeg, aMinPoint, bMinPoint, aMinRatio, bMinRatio);
		CheckCandidate(aMinPoint, bMinPoint);
	};

	// x方向の4辺
	CheckEdge({ boxMin.x, boxMin.y, boxMin.z }, { boxMax.x, boxMin.y, boxMin.z });
	CheckEdge({ boxMin.x, boxMin.y, boxMax.z }, { boxMax.x, boxMin.y, boxMax.z });
	CheckEdge({ boxMin.x, boxMax.y, boxMin.z }, { boxMax.x, boxMax.y, boxMin.z });
	CheckEdge({ boxMin.x, boxMax.y, boxMax.z }, { boxMax.x, boxMax.y, boxMax.z });

	// y方向の4辺
	CheckEdge({ boxMin.x, boxMin.y, boxMin.z }, { boxMin.x, boxMax.y, boxMin.z });
	CheckEdge({ boxMax.x, boxMin.y, boxMin.z }, { boxMax.x, boxMax.y, boxMin.z });
	CheckEdge({ boxMin.x, boxMin.y, boxMax.z }, { boxMin.x, boxMax.y, boxMax.z });
	CheckEdge({ boxMax.x, boxMin.y, boxMax.z }, { boxMax.x, boxMax.y, boxMax.z });

	// z方向の4辺
	CheckEdge({ boxMin.x, boxMin.y, boxMin.z }, { boxMin.x, boxMin.y, boxMax.z });
	CheckEdge({ boxMax.x, boxMin.y, boxMin.z }, { boxMax.x, boxMin.y, boxMax.z });
	CheckEdge({ boxMin.x, boxMax.y, boxMin.z }, { boxMin.x, boxMax.y, boxMax.z });
	CheckEdge({ boxMax.x, boxMax.y, boxMin.z }, { boxMax.x, boxMax.y, boxMax.z });

	// AABBの6面から線分への交点

	// 各面から線分への交点
	auto CheckFace = [&](float coord, int i) {
		float dirAxis = segDir[i];
		// 限りなく平行に近いなら無効
		if (fabs(dirAxis) < EPSILON) return;

		// 線分上の面と交わる位置
		float intersectRatio = (coord - segStart[i]) / dirAxis;
		// 線分外なら無効
		if (intersectRatio < 0.0f || intersectRatio > 1.0f) return;

		// 面と交わる座標
		Vector3 segPoint = segStart + segDir * intersectRatio;
		// チェック中以外の軸
		int axisA = (i + 1) % 3;
		int axisB = (i + 2) % 3;

		// 他の軸がAABB内に収まっているかどうか
		if (segPoint[axisA] < boxMin[axisA] || segPoint[axisA] > boxMax[axisA]) return;
		if (segPoint[axisB] < boxMin[axisB] || segPoint[axisB] > boxMax[axisB]) return;
		
		CheckCandidate(segPoint, segPoint);
	};

	// 6つの面を順番に確認
	for (int i = 0; i < 3; i++) {
		CheckFace(boxMin[i], i);
		CheckFace(boxMax[i], i);
	}

	return;
}

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
 * カプセル同士の交差判定
 * @param[in]  Capsule a   判定対象1
 * @param[in]  Capsule b   判定対象2
 * @param[out] Vector3 penetration 貫通ベクトル
 */
bool Intersect(const Capsule & a, const Capsule & b, Vector3 & penetration) {
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
	// 最近接点間の長さが限りなく0に近い場合は中心点間の方向で比較
	else {
		Vector3 aCenter = (a.segment.startPoint + a.segment.endPoint) * 0.5f;
		Vector3 bCenter = (b.segment.startPoint + b.segment.endPoint) * 0.5f;
		Vector3 centerDir = bCenter - aCenter;

		float cdx = fabsf(centerDir.x);
		float cdy = fabsf(centerDir.y);
		float cdz = fabsf(centerDir.z);

		if (cdx > cdy && cdx > cdz) {
			direction = V_RIGHT;
			direction.x = centerDir.x >= 0 ? 1 : -1;
		}
		else if (cdy > cdz) {
			direction = V_UP;
			direction.y = centerDir.y >= 0 ? 1 : -1;
		}
		else {
			direction = V_FORWARD;
			direction.z = centerDir.z >= 0 ? 1 : -1;
		}
	}
	// 貫通深度
	float depth = sumRadius - length;
	if (depth < 0) depth = 0;
	// 貫通距離
	penetration = direction * depth;
	return true;
}

/*
 *  カプセル対AABBの交差判定
 *  @param[in]  Capsule cap			判定対象1
 *  @param[in]  AABB	box			判定対象2
 *  @param[out] Vector3 penetration 貫通ベクトル
 */
bool Intersect(const Capsule& capsule, const AABB& box, Vector3& penetration) {
	Vector3 segMinPoint, boxMinPoint;
	float minLengthSq;
	// カプセルの線分とAABBの最近接点を求める
	SegmentToAABBMinLength(capsule.segment, box, segMinPoint, boxMinPoint, minLengthSq);

	// カプセルの半径とその2乗
	float radius = capsule.radius;
	float radiusSq = radius * radius;

	// 最近接点間の長さがカプセルの半径以上なら交差していない
	if (minLengthSq >= radiusSq) return false;

	// 貫通距離を出す
	// 2つの最近接点間の差ベクトル
	Vector3 difference = segMinPoint - boxMinPoint;
	// 最近接点間の長さ
	float minLength = sqrt(minLengthSq);

	Vector3 direction;
	// 最近接点間の長さが0に限りなく近くなければ正規化
	if (minLength > EPSILON) {
		direction = difference / minLength;
	}
	// 最近接点間の長さが限りなく0に近い場合はAABBの中心からカプセル中心方向で比較
	else {
		Vector3 boxCenter = (box.min + box.max) * 0.5f;
		Vector3 capCenter = (capsule.segment.startPoint + capsule.segment.endPoint) * 0.5f;
		Vector3 centerDir = capCenter - boxCenter;

		float cdx = fabsf(centerDir.x);
		float cdy = fabsf(centerDir.y);
		float cdz = fabsf(centerDir.z);

		// 最も成分の大きい軸に固定
		if (cdx > cdy && cdx > cdz) {
			direction = V_RIGHT;
			direction.x = centerDir.x >= 0 ? 1 : -1;
		}
		else if (cdy > cdz) {
			direction = V_UP;
			direction.y = centerDir.y >= 0 ? 1 : -1;
		}
		else {
			direction = V_FORWARD;
			direction.z = centerDir.z >= 0 ? 1 : -1;
		}
	}
	// 貫通深度
	float depth = radius - minLength;
	if (depth < 0) depth = 0;
	// 貫通距離
	penetration = direction * depth;
	return true;
}
/*
 *  AABB対カプセルの交差判定
 *  @param[in]  AABB	box			判定対象1
 *  @param[in]  Capsule cap			判定対象2
 *  @param[out] Vector3 penetration 貫通ベクトル
 */
bool Intersect(const AABB& box, const Capsule& capsule, Vector3& penetration) {
	return Intersect(capsule, box, penetration);
}

/*
 *  コライダー同士の交差判定
 *  @param[in]  std::variant<AABB, Capsule> a			判定対象1
 *  @param[in]  std::variant<AABB, Capsule> b			判定対象2
 *  @param[out] Vector3						penetration	貫通ベクトル
 */
bool Intersect(const std::variant<AABB, Capsule>& a, const std::variant<AABB, Capsule>& b, Vector3& penetration) {
	// 各対象のコライダーを判別し、それに合った交差判定を呼ぶ
	if (auto aabbA = std::get_if<AABB>(&a)) {
		if (auto aabbB = std::get_if<AABB>(&b)) {
			return Intersect(*aabbA, *aabbB, penetration);
		}
		else if (auto capsuleB = std::get_if<Capsule>(&b)) {
			return Intersect(*aabbA, *capsuleB, penetration);
		}
	}
	else if (auto capsuleA = std::get_if<Capsule>(&a)) {
		if (auto aabbB = std::get_if<AABB>(&b)) {
			return Intersect(*capsuleA, *aabbB, penetration);
		}
		else if (auto capsuleB = std::get_if<Capsule>(&b)) {
			return Intersect(*capsuleA, *capsuleB, penetration);
		}
	}
}

/*
 *  レイとAABBの交差判定
 *  @param[in]  Ray		ray			判定対象1
 *  @param[in]  AABB	box			判定対象2
 *  @param[out] float	distance	交点までの距離
 */
bool RayIntersect(const Ray& ray, const AABB& box, float& distance) {
	float segMinRatio = 0;
	float segMaxRatio = MAX_DISTANCE;

	// xスラブ
	if (!IntersectSlab(ray.start.x, ray.direction.x, box.min.x, box.max.x, segMinRatio, segMaxRatio))
		return false;

	// yスラブ
	if (!IntersectSlab(ray.start.y, ray.direction.y, box.min.y, box.max.y, segMinRatio, segMaxRatio))
		return false;

	// zスラブ
	if (!IntersectSlab(ray.start.z, ray.direction.z, box.min.z, box.max.z, segMinRatio, segMaxRatio))
		return false;

	// 交点までの距離を保存
	distance = segMinRatio;
	return true;
}

/*
 *  レイとカプセルの交差判定
 *  @param[in]	Ray		ray			判定対象1
 *  @param[in]	Capsule	capsule		判定対象2
 *	@param[out]	float	distance	交点までの距離
 */
bool RayIntersect(const Ray& ray, const Capsule& capsule, float& distance) {
	Vector3 aMinPoint, bMinPoint;
	float aMinRatio, bMinRatio;
	Segment raySeg = { ray.start, ray.start + ray.direction * MAX_DISTANCE };
	// レイとカプセルの線分の最近接点を求める
	SegmentBetweenMinLength(raySeg, capsule.segment, aMinPoint, bMinPoint, aMinRatio, bMinRatio);
	// 2つの最近接点の差ベクトル
	Vector3 difference = aMinPoint - bMinPoint;
	// 最近接点間の長さの2乗(比較の時点では2乗で十分なので今はsqrtしない)
	float lengthSquare = Dot(difference, difference);
	// カプセルの半径の2乗
	float radiusSquare = capsule.radius * capsule.radius;

	// 最近接点間の長さがカプセルの半径の合計以上なら衝突はしていない
	if (lengthSquare >= radiusSquare) return false;

	// 交点までの距離を求める
	// レイの開始点から最近接点までの距離
	float rayMinDistance = aMinRatio * MAX_DISTANCE;
	// 半径から最近接点間の長さを引いた距離
	float offset = std::sqrt(radiusSquare - lengthSquare);
	// カプセルの半径から最近接点間の距離を引いた値を半径とし、中心点をレイの最近接点とする円上には
	// レイの侵入点と脱出点、最近接点間を線で結んだときにレイの後ろ側にあるカプセルの表面点の3点が存在する
	// なのでレイの開始点から最近接点までの距離から半径から最近接点間の長さを引いた距離を引いた値がレイの侵入点となる
	distance = rayMinDistance - offset;
	if (distance < 0.0f) distance = 0;

	return true;
}

/*
 *  レイとコライダーの交差判定
 *  @param[in]	Ray							ray			判定対象1
 *  @param[in]	std::variant<AABB, Capsule>	collider	判定対象2
 *	@param[out]	float						distance	交点までの距離
 */
bool RayIntersect(const Ray& ray, const std::variant<AABB, Capsule>& collider, float& distance) {
	// 各対象のコライダーを判別し、それに合った交差判定を呼ぶ
	if (auto aabb = std::get_if<AABB>(&collider)) {
		return RayIntersect(ray, *aabb, distance);
	}
	else if (auto capsule = std::get_if<Capsule>(&collider)) {
		return RayIntersect(ray, *capsule, distance);
	}
}
