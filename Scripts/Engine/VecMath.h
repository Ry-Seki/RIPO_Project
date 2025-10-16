/*
 *	@file	VecMath.h
 *	@author	Seki
 */

#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <cmath>
#include "DxLib.h"  
#define Pi (3.14159265f)
#define Deg2Rad (Pi / 180.0f)
#define Rad2Deg (180.0f / Pi)

 //	Vector2
struct Vector2 { float x, y; };
//	Vector3
struct Vector3 {
	// メンバ変数
	float x, y, z;

	// 静的メンバ変数
	static Vector3 zero, one;
	static Vector3 up, down, right, left, forward, back;

	// コンストラクタ
	Vector3();
	Vector3(float setX, float setY, float setZ);
	~Vector3() = default;

	// DxLibのVECTORとの互換関数
	static VECTOR ToVECTOR(const Vector3& v) {
		return VGet(v.x, v.y, v.z);
	}

	static Vector3 FromVECTOR(const VECTOR& v) {
		return { v.x, v.y, v.z };
	}

	// 演算子
	Vector3 operator - () {
		return { -x, -y, -z };
	}

	Vector3 operator * (float v) const {
		return { x * v, y * v, z * v };
	}
	Vector3 operator / (float v) const {
		return { x / v, y / v, z / v };
	};
	Vector3 operator + (const Vector3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	};
	Vector3 operator - (const Vector3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	};

	Vector3& operator += (const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	};
	Vector3& operator -= (const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	};
	Vector3& operator *= (float v) {
		x *= v;
		y *= v;
		z *= v;

		return *this;
	};
	Vector3& operator /= (float v) {
		x /= v;
		y /= v;
		z /= v;

		return *this;
	};

	bool operator == (const Vector3& v) const {
		return x == v.x && y == v.y && z == v.z;
	};
	bool operator != (const Vector3& v) const {
		return !(*this == v);
	};

	// 演算
	static Vector3 Scale(const Vector3& v1, const Vector3& v2) {
		return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
	}
	static float Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}

	float Magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	};
	Vector3 Normalized() const {
		float magnitude = Magnitude();
		if (magnitude == 0) return Vector3::zero;
		return { x / magnitude, y / magnitude, z / magnitude };
	};

	// 線形補間
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
		return a + (b - a) * t;
	}

	// 球面線形補間
	static Vector3 Slerp(const Vector3& a, const Vector3& b, float t) {
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

	// Vector3のClamp
	static Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max) {
		return {
			std::fmax(min.x, std::fmin(max.x, v.x)),
			std::fmax(min.y, std::fmin(max.y, v.y)),
			std::fmax(min.z, std::fmin(max.z, v.z))
		};
	}

	// floatのClamp
	static float Clamp(float value, float min, float max) {
		return std::fmax(min, std::fmin(max, value));
	}

	// fabs 浮動小数点数の絶対値を求める
	static float fabs(float v) {
		return (v < 0.0f) ? -v : v;
	}

	// 2点間の距離を求める
	static float Distance(const Vector3& a, const Vector3& b) {
		return (a - b).Magnitude();
	}

};
#endif // !_VECMATH_H_
