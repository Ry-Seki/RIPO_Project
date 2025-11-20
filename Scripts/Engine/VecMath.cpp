/*
 *	@file	VecMath.cpp
 *	@author	Seki
 *  @author oorui
 */

#include "VecMath.h"

 // 静的メンバ変数の宣言
Vector3 Vector3::zero = { 0.0f, 0.0f, 0.0f };
Vector3 Vector3::one = { 1.0f, 1.0f, 1.0f };
Vector3 Vector3::up = { 0.0f, 1.0f, 0.0f };
Vector3 Vector3::down = { 0.0f,-1.0f, 0.0f };
Vector3 Vector3::right = { 1.0f, 0.0f, 0.0f };
Vector3 Vector3::left = { -1.0f, 0.0f, 0.0f };
Vector3 Vector3::forward = { 0.0f, 0.0f, 1.0f };
Vector3 Vector3::back = { 0.0f, 0.0f,-1.0f };

/*
 *	コンストラクタ
 */
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
/*
 *	コンストラクタのオーバーロード
 */
Vector3::Vector3(float setX, float setY, float setZ) : x(setX), y(setY), z(setZ) {}


/*
 *	Vector3型からVECTOR型への型変換
 *  @param		v	Vector3型
 */
VECTOR Vector3::ToVECTOR(const Vector3& v) {
	return VGet(v.x, v.y, v.z);
}
/*
 *	VECTOR型からVector3型への型変換
 *  @param		v	VECTOR型
 */
Vector3 Vector3::FromVECTOR(const VECTOR& v) {
	return { v.x, v.y, v.z };
}


/*
 *	各成分毎の乗算
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return Vector3
 */
Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

/*
 *	内積
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return float
 */
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 *	外積
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return Vector3
 */
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

/*
 *	ベクトルの長さ
 *  @return float
 */
float Vector3::Magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

/*
 *	正規化
 *  @return Vector3
 */
Vector3 Vector3::Normalized() const {
	float magnitude = Magnitude();
	if (magnitude == 0) return Vector3::zero;
	return { x / magnitude, y / magnitude, z / magnitude };
}

/*
 *	2点間( v1 , v2 )の距離
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return float
 */
float Vector3::Distance(const Vector3& v1, const Vector3& v2) {
	return (v1 - v2).Magnitude();
}

/*
 *	2点間の方向を表す単位ベクトル
 *  @param[in]	from	1つ目のベクトル
 *  @param[in]	to		2つ目のベクトル
 *  @return Vector3
 */
Vector3 Vector3::Direction(const Vector3& from, const Vector3& to) {
	Vector3 dir = to - from;
	return dir.Normalized();
}

/*
 *	正面方向のベクトル
 *  @param[in]	rotation	回転
 *  @return Vector3
 *  @author Riku
 */
Vector3 Vector3::ForwardDir(const Vector3& rotation) {
	Vector3 dir;
	float rotX = rotation.x;
	float rotY = rotation.y;
	dir.x = cos(rotX) * sin(rotY);
	dir.y = -sin(rotX);
	dir.z = cos(rotX) * cos(rotY);
	return dir.Normalized();
}

/*
 *	各成分を指定範囲内に制限する
 *	@param[in]  v1   対象のベクトル
 *	@param[in]  min  下限値ベクトル
 *	@param[in]  max  上限値ベクトル
 *  @return Vector3
 */
Vector3 Vector3::Clamp(const Vector3& v1, const Vector3& min, const Vector3& max) {
	return {
	std::fmax(min.x, std::fmin(max.x, v1.x)),
	std::fmax(min.y, std::fmin(max.y, v1.y)),
	std::fmax(min.z, std::fmin(max.z, v1.z))
	};
}

/*
 *  値を指定範囲内に制限する
 *  @param[in]  value 対象の値
 *  @param[in]  min   下限値
 *  @param[in]  max   上限値
 *  @return	float
 */
float Vector3::Clamp(float value, float min, float max) {
	return std::fmax(min, std::fmin(max, value));
}

/*
 *	2つのベクトル間の差分を求める
 *	@param[in]  v1  基準となるベクトル
 *	@param[in]  v2  引かれるベクトル
 *	@return	Vector3
 */
Vector3 Vector3::Offset(const Vector3& v1, const Vector3& v2) {
	return (v1 - v2);
}

/*
 *	線形補間
 *  @param[in]	v1	始点のベクトル
 *  @param[in]	v2	終点のベクトル
 *  @param[in]	t	補間係数（0.0f~1.0f）
 *  @return Vector3
 */
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 + (v2 - v1) * t;
}

/*
 *	球面線形補間
 *  @param[in] v1		  始点のベクトル
 *  @param[in] v2		  終点のベクトル
 *  @param[in] t		  補間係数（0.0f~1.0f）
 *  @return Vector3
 */
Vector3 Vector3::Slerp(const Vector3& a, const Vector3& b, float t) {
	// tを[ 0 , 1 ]にする
	t = Clamp(t, 0.0f, 1.0f);

	float magA = a.Magnitude();
	float magB = b.Magnitude();
	// 片方でも0だったら線形補間を利用
	if (magA == 0 || magB == 0) {
		return Lerp(a, b, t);
	}

	// 正規化
	Vector3 v0 = a.Normalized();
	Vector3 v1 = b.Normalized();

	// 角度
	float dot = Dot(v0, v1);
	dot = Clamp(dot, -1.0f, 1.0f);

	// dot < 0 の場合、反転して最短経路を選ぶ
	if (dot < 0.0f) {
		dot = -dot;
		v1 = -v1;
	}

	// 角度の計算
	float theta = std::acos(dot);      // ベクトル間の角度:radian
	float sinTheta = std::sin(theta);  // sinThetaの計算

	// 球面補間の係数を計算
	float w0 = std::sin((1.0f - t) * theta) / sinTheta;
	float w1 = std::sin(t * theta) / sinTheta;


	// 方向ベクトルを足す
	Vector3 dir = v0 * w0 + v1 * w1;

	// 線形に補間する
	float len = magA + (magB - magA) * t;

	return dir * len;
}

/*
 *	最近接点を求める
 *  @param[in]	v1			球の中心点
 *  @param[in]  v2,v3,v4	三角形の3頂点
 */
Vector3 Vector3::Nearest(const Vector3& senterPos, const Vector3& v1, const Vector3& v2, const Vector3& v3) {
	// 各頂点の要素1 -> 要素2の辺を求める
	Vector3 ab = v2 - v1;
	Vector3 ac = v3 - v1;
	Vector3 ap = senterPos - v1;

	// 頂点v1の判定
	// 中心点と辺の位置関係
	float d1 = Dot(ab, ap);
	float d2 = Dot(ac, ap);

	// 中心点が三角形の外だったら
	if (d1 <= 0.0f && d2 <= 0.0f) {
		// 頂点 v1 が最近接点
		return v1;
	}

	// 頂点v2の判定
	Vector3 bp = senterPos - v2;
	// 中心点と辺の位置関係
	float d3 = Dot(ab, bp);
	float d4 = Dot(ac, bp);
	// 中心点が三角形の外だったら
	if (d3 >= 0.0f && d4 <= d3) {
		// 頂点 v2 が最近接点
		return v2;
	}

	// ab辺の最近接点を求める
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		// a->b方向の補間係数
		float t = d1 / (d1 - d3);
		// 辺ab上の最近接点
		return v1 + ab * t;
	}

	// 頂点v3の判定
	Vector3 cp = senterPos - v3;
	// 中心点と辺の位置関係
	float d5 = Dot(ab, cp);
	float d6 = Dot(ac, cp);
	// 中心点が三角形の外だったら
	if (d6 >= 0.0f && d5 <= d6) {
		// 頂点v3が最近接点
		return v3;
	}

	// ac辺の最近接点を求める
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		// acの補間係数
		float t = d2 / (d2 - d6);
		// 辺acの最近接点
		return v1 + ac * t;
	}

	// bcの最近接点を求める
	Vector3 bc = v3 - v2;
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		// bcの補間係数
		float t = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		// 辺bcの最近接点
		return v2 + bc * t;
	}

	// それ以外だったら
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	// 三角形に存在する最近接点
	return v1 + ab * v + ac * w;

}


// 比較
bool operator==(const Vector3& a, const Vector3& b) {
	const float epsilon = 1e-6f;
	return (std::fabs(a.x - b.x) < epsilon) &&
		(std::fabs(a.y - b.y) < epsilon) &&
		(std::fabs(a.z - b.z) < epsilon);
}

bool operator!=(const Vector3& a, const Vector3& b) {
	return !(a == b);
}

// 四則演算
Vector3 operator+(const Vector3& a, const Vector3& b) {
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator*(const Vector3& a, float s) {
	return { a.x * s, a.y * s, a.z * s };
}

Vector3 operator/(const Vector3& a, float s) {
	return { a.x / s, a.y / s, a.z / s };
}

Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}


// 複合代入演算
Vector3& operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

Vector3& operator-=(Vector3& lhs, const Vector3& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

Vector3& operator*=(Vector3& lhs, float s) {
	lhs.x *= s;
	lhs.y *= s;
	lhs.z *= s;
	return lhs;
}

Vector3& operator/=(Vector3& lhs, float s) {
	lhs.x /= s;
	lhs.y /= s;
	lhs.z /= s;
	return lhs;
}

Vector3 Scale(const Vector3& v1, const Vector3& v2) {
	return Vector3::Scale(v1, v2);
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return Vector3::Dot(v1, v2);
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3::Cross(v1, v2);
}

float Magnitude(const Vector3& v) {
	return  v.Magnitude();
}

Vector3 Normalized(const Vector3& v) {
	return v.Normalized();
}

float Distance(const Vector3& v1, const Vector3& v2) {
	return Vector3::Distance(v1, v2);
}

Vector3 Direction(const Vector3& from, const Vector3& to) {
	return Vector3::Direction(from, to);
}

Vector3 ForwardDir(const Vector3& rotation) {
	return Vector3::ForwardDir(rotation);
}

Vector3 Offset(const Vector3& v1, const Vector3& v2) {
	return Vector3::Offset(v1, v2);
}

Vector3 Clamp(const Vector3& v1, const Vector3& min, const Vector3& max) {
	return Vector3::Clamp(v1, min, max);
}

float Clamp(float value, float min, float max) {
	return Vector3::Clamp(value, min, max);
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Vector3::Lerp(v1, v2, t);
}

Vector3 Slerp(const Vector3& a, const Vector3& b, float t) {
	return Vector3::Slerp(a, b, t);
}

Vector3 Nearest(const Vector3& senterPos, const Vector3& v1, const Vector3& v2, const Vector3& v3) {
	return Vector3::Nearest(senterPos, v1, v2, v3);
}

const Vector3 V_ZERO = Vector3::zero;
const Vector3 V_ONE = Vector3::one;
const Vector3 V_UP = Vector3::up;
const Vector3 V_DOWN = Vector3::down;
const Vector3 V_RIGHT = Vector3::right;
const Vector3 V_LEFT = Vector3::left;
const Vector3 V_FORWARD = Vector3::forward;
const Vector3 V_BACK = Vector3::back;

VECTOR ToVECTOR(const Vector3& v) {
	return Vector3::ToVECTOR(v);
}

Vector3 FromVECTOR(const VECTOR& v) {
	return Vector3::FromVECTOR(v);
}