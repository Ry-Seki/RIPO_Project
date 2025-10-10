/*
 *	@file	VecMath.h
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

		return { x / magnitude, y / magnitude, z / magnitude };
	};
	
};
#endif // !_VECMATH_H_
