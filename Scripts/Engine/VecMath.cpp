/*
 *	@file	VecMath.cpp
 *	@author	Seki
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


// DxLibのVECTORとの互換関数
VECTOR Vector3::ToVECTOR(const Vector3& v) {
	return VGet(v.x, v.y, v.z);
}
Vector3 Vector3::FromVECTOR(const VECTOR& v) {
	return { v.x, v.y, v.z };
}


/*
 *	演算
 */

 /*
  *	各成分毎の乗算
  */
Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

/*
 *	内積
 */
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 *	外積
 */
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}
/*
 *	長さ
 */
float Vector3::Magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}
/*
 *	正規化
 */
Vector3 Vector3::Normalized() const {
	float magnitude = Magnitude();
	if (magnitude == 0) return Vector3::zero;
	return { x / magnitude, y / magnitude, z / magnitude };
}

/*
 *	2点間の長さ
 */
float Vector3::Distance(const Vector3& v1, const Vector3& v2) {
	return (v1 - v2).Magnitude();
}

/*
 *	方向ベクトル
 */
Vector3 Vector3::Direction(const Vector3& from, const Vector3& to) {
	Vector3 dir = to - from;
	return dir.Normalized();
}


Vector3 Vector3::Clamp(const Vector3& v1, const Vector3& min, const Vector3& max) {
	return {
	std::fmax(min.x, std::fmin(max.x, v1.x)),
	std::fmax(min.y, std::fmin(max.y, v1.y)),
	std::fmax(min.z, std::fmin(max.z, v1.z))
	};
}

float Vector3::Clamp(float value, float min, float max) {
	return std::fmax(min, std::fmin(max, value));
}

/*
 *	線形補間
 */
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 + (v2 - v1) * t;
}

/*
 *	球面線形補間
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