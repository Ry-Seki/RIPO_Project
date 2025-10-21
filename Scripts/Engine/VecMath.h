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
	static VECTOR ToVECTOR(const Vector3& v);
	static Vector3 FromVECTOR(const VECTOR& v);

	// 演算
	// 各成分の乗算
	static Vector3 Scale(const Vector3& v1, const Vector3& v2);
	// 内積
	static float Dot(const Vector3& v1, const Vector3& v2);
	// 外積
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// 長さ
	float Magnitude() const;
	// 正規化
	Vector3 Normalized() const;

	static float Distance(const Vector3& v1, const Vector3& v2);
	static Vector3 Direction(const Vector3& from, const Vector3& to);
	static Vector3 Clamp(const Vector3& v1, const Vector3& min, const Vector3& max);
	static float Clamp(float value, float min, float max);

	// 線形補間
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// 球面線形補間
	static Vector3 Slerp(const Vector3& a, const Vector3& b, float t);
};


bool operator==(const Vector3& a, const Vector3& b);
bool operator!=(const Vector3& a, const Vector3& b);


Vector3 operator+(const Vector3& a, const Vector3& b);
Vector3 operator-(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& a, float s);
Vector3 operator/(const Vector3& a, float s);
Vector3 operator-(const Vector3& v);

Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
Vector3& operator*=(Vector3& lhs, float s);
Vector3& operator/=(Vector3& lhs, float s);

#endif // !_VECMATH_H_
