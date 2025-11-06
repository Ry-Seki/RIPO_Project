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
	static Vector3 zero;
	static Vector3 one;
	static Vector3 up;
	static Vector3 down;
	static Vector3 right;
	static Vector3 left;
	static Vector3 forward;
	static Vector3 back;

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
	static Vector3 ForwardDir(const Vector3& rotation);
	static Vector3 Clamp(const Vector3& v1, const Vector3& min, const Vector3& max);
	static float Clamp(float value, float min, float max);
	static Vector3 Offset(const Vector3& v1, const Vector3& v2);
	// 線形補間
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// 球面線形補間
	static Vector3 Slerp(const Vector3& a, const Vector3& b, float t);
};

/*
 *	非メンバ演算子の宣言
 */
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


/*
 *	非メンバ演算関数の宣言
 */
Vector3 Scale(const Vector3& v1, const Vector3& v2);
float   Dot(const Vector3& v1, const Vector3& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);

float   Magnitude(const Vector3& v);
Vector3 Normalized(const Vector3& v);
float   Distance(const Vector3& v1, const Vector3& v2);
Vector3 Direction(const Vector3& from, const Vector3& to);
Vector3 Offset(const Vector3& v1, const Vector3& v2);

Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max);
float   Clamp(float value, float min, float max);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& a, const Vector3& b, float t);

/*
 *	定数ベクトル
 */
extern const Vector3 V_ZERO;
extern const Vector3 V_ONE;
extern const Vector3 V_UP;
extern const Vector3 V_DOWN;
extern const Vector3 V_RIGHT;
extern const Vector3 V_LEFT;
extern const Vector3 V_FORWARD;
extern const Vector3 V_BACK;

/*
 *	DxLib VECTOR 互換関数
 */
VECTOR ToVECTOR(const Vector3& v);
Vector3 FromVECTOR(const VECTOR& v);

#endif // !_VECMATH_H_
