/*
 *	@file	VecMath.h
 *	@author	Seki
 */

#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <cmath>

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

	// 演算子
	Vector3 operator - () {
		return { -x, -y, -z };
	}

	Vector3 operator * (float v) const {
		return { x * v, y * v, z * v };
	}
	Vector3 operator / (float v) const {
		return { x / v, y * v, z * v };
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
		if (*this != v) false;

		return true;
	};
	bool operator != (const Vector3& v) const {
		if (*this == v) false;

		return true;
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
		// 正規化
		Vector3 v0 = a.Normalized();
		Vector3 v1 = b.Normalized();

		// 角度
		float dot = Dot(v0, v1);
		dot = Clamp(dot, -1.0f, 1.0f);

		// 角度をラジアン角に変換
		float rad = std::acos(dot) * t;

		// 補間方向を求める
		Vector3 iDir = (v1 - v0 * dot).Normalized();

		// 球面補間の式
		return v0 * std::cos(rad) + iDir * std::sin(rad);

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


};
#endif // !_VECMATH_H_
