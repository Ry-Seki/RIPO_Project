/*
 *	@file	VecMath.h
 *	@author	Seki
 *  @author oorui
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
	static Vector3 zero;		// {  0.0f, 0.0f, 0.0f }
	static Vector3 one;			// {  1.0f, 1.0f, 1.0f }
	static Vector3 up;			// {  0.0f, 1.0f, 0.0f }
	static Vector3 down;		// {  0.0f,-1.0f, 0.0f }
	static Vector3 right;		// {  1.0f, 0.0f, 0.0f }
	static Vector3 left;		// { -1.0f, 0.0f, 0.0f }
	static Vector3 forward;		// {  0.0f, 0.0f, 1.0f }
	static Vector3 back;		// {  0.0f, 0.0f,-1.0f }

	bool operator==(const Vector3& v)const {
		return { x == v.x && y == v.y && z == v.z };
	};
	bool operator!=(const Vector3& v)const {
		return { x != v.x && y != v.y && z != v.z };
	};

	Vector3 operator+(const Vector3& v)const {
		return { x + v.x, y + v.y, z + v.z };
	};
	Vector3 operator-(const Vector3& v)const {
		return { x - v.x, y - v.y, z - v.z };
	};
	Vector3 operator*(float v)const {
		return { x * v, y * v, z * v };
	};
	Vector3 operator/(float v) {
		return { x / v, y / v, z / v };
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



	Vector3 operator-() {
		return { -x,-y,-z };
	};

	float& operator [] (int index) {
		if (index < 3)
			return *(&x + index);
	}

	/*
	 *	コンストラクタ
	 */
	Vector3();
	/*
	 *	コンストラクタ
	 *  @param setX		X成分
	 *  @param setY		Y成分
	 *  @param setZ		Z成分
	 */
	Vector3(float setX, float setY, float setZ);

	Vector3(VECTOR v)
		: x(v.x), y(v.y), z(v.z) {
	}

	/*
	 *	デストラクタ
	 */
	~Vector3() = default;

	/*
	 *	Vector3型からVECTOR型への型変換
	 *  @param		v	Vector3型
	 */
	static VECTOR ToVECTOR(const Vector3& v);

	/*
	 *	VECTOR型からVector3型への型変換
	 *  @param		v	VECTOR型
	 */
	static Vector3 FromVECTOR(const VECTOR& v);

	// 演算
	/*
	 *	各成分毎の乗算
	 *  @param[in]	v1	1つ目のベクトル
	 *  @param[in]	v2	2つ目のベクトル
	 *  @return Vector3
	 */
	static Vector3 Scale(const Vector3& v1, const Vector3& v2);

	/*
	 *	内積
	 *  @param[in]	v1	1つ目のベクトル
	 *  @param[in]	v2	2つ目のベクトル
	 *  @return float
	 */
	static float Dot(const Vector3& v1, const Vector3& v2);

	/*
	 *	外積
	 *  @param[in]	v1	1つ目のベクトル
	 *  @param[in]	v2	2つ目のベクトル
	 *  @return Vector3
	 */
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/*
	 *	ベクトルの長さ
	 *  @return float
	 */
	float Magnitude() const;

	/*
	 *	正規化
	 *  @return Vector3
	 */
	Vector3 Normalized() const;

	/*
	 *	2点間( v1 , v2 )の距離
	 *  @param[in]	v1	1つ目のベクトル
	 *  @param[in]	v2	2つ目のベクトル
	 *  @return float
	 */
	static float Distance(const Vector3& v1, const Vector3& v2);

	/*
	 *	2点間の方向を表す単位ベクトル
	 *  @param[in]	from	1つ目のベクトル
	 *  @param[in]	to		2つ目のベクトル
	 *  @return Vector3
	 */
	static Vector3 Direction(const Vector3& from, const Vector3& to);

	/*
	 *	正面方向のベクトル
	 *  @param[in]	rotation	回転
	 *  @return Vector3
	 */
	static Vector3 ForwardDir(const Vector3& rotation);

	/*
	 *	各成分を指定範囲内に制限する
	 *	@param[in]  v1   対象のベクトル
	 *	@param[in]  min  下限値ベクトル
	 *	@param[in]  max  上限値ベクトル
	 *  @return Vector3
	 */
	static Vector3 Clamp(const Vector3& v1, const Vector3& min, const Vector3& max);
	/*
	 *  値を指定範囲内に制限する
	 *  @param[in]  value 対象の値
	 *  @param[in]  min   下限値
	 *  @param[in]  max   上限値
	 *  @return	float
	 */
	static float Clamp(float value, float min, float max);

	/*
	 *	2つのベクトル間の差分を求める
	 *	@param[in]  v1  基準となるベクトル
	 *	@param[in]  v2  引かれるベクトル
	 *	@return	Vector3
	 */
	static Vector3 Offset(const Vector3& v1, const Vector3& v2);

	/*
	 *	線形補間
	 *  @param[in]	v1	始点のベクトル
	 *  @param[in]	v2	終点のベクトル
	 *  @param[in]	t	補間係数（0.0f~1.0f）
	 *  @return Vector3
	 */
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	/*
	 *	球面線形補間
	 *  @param[in] v1		  始点のベクトル
	 *  @param[in] v2		  終点のベクトル
	 *  @param[in] t		  補間係数（0.0f~1.0f）
	 *  @return Vector3
	 */
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

	/*
	 *	最近接点を求める
	 *  @param[in]	v1			球の中心点
	 *  @param[in]  v2,v3,v4	三角形の3頂点
	 */
	static Vector3 Nearest(const Vector3& senterPos, const Vector3& v1, const Vector3& v2, const Vector3& v3);
};

/*
 *	非メンバ演算関数の宣言
 */

 /*
  *	各成分毎の乗算
  *  @param[in]	v1	1つ目のベクトル
  *  @param[in]	v2	2つ目のベクトル
  *  @return Vector3
  */
Vector3 Scale(const Vector3& v1, const Vector3& v2);

/*
 *	内積
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return float
 */
float   Dot(const Vector3& v1, const Vector3& v2);

/*
 *	外積
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return Vector3
 */
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/*
 *	ベクトルの長さ
 *  @return float
 */
float   Magnitude(const Vector3& v);

/*
 *	正規化
 *  @return Vector3
 */
Vector3 Normalized(const Vector3& v);

/*
 *	2点間( v1 , v2 )の距離
 *  @param[in]	v1	1つ目のベクトル
 *  @param[in]	v2	2つ目のベクトル
 *  @return float
 */
float   Distance(const Vector3& v1, const Vector3& v2);

/*
 *	2点間の方向を表す単位ベクトル
 *  @param[in]	from	1つ目のベクトル
 *  @param[in]	to		2つ目のベクトル
 *  @return Vector3
 */
Vector3 Direction(const Vector3& from, const Vector3& to);


/*
 *	2つのベクトル間の差分を求める
 *	@param[in]  v1  基準となるベクトル
 *	@param[in]  v2  引かれるベクトル
 *	@return	Vector3
 */
Vector3 Offset(const Vector3& v1, const Vector3& v2);


/*
 *	正面方向のベクトル
 *  @param[in]	rotation	回転
 *  @return Vector3
 */
Vector3 ForwardDir(const Vector3& rotation);

/*
 *	各成分を指定範囲内に制限する
 *	@param[in]  v1   対象のベクトル
 *	@param[in]  min  下限値ベクトル
 *	@param[in]  max  上限値ベクトル
 *  @return Vector3
 */
Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max);

/*
 *  値を指定範囲内に制限する
 *  @param[in]  value 対象の値
 *  @param[in]  min   下限値
 *  @param[in]  max   上限値
 *  @return	float
 */
float   Clamp(float value, float min, float max);

/*
 *	線形補間
 *  @param[in]	v1	始点のベクトル
 *  @param[in]	v2	終点のベクトル
 *  @param[in]	t	補間係数（0.0f~1.0f）
 *  @return Vector3
 */
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/*
 *	球面線形補間
 *  @param[in] v1		  始点のベクトル
 *  @param[in] v2		  終点のベクトル
 *  @param[in] t		  補間係数（0.0f~1.0f）
 *  @return Vector3
 */
Vector3 Slerp(const Vector3& a, const Vector3& b, float t);

/*
 *	最近接点を求める
 *  @param[in]	v1			球の中心点
 *  @param[in]  v2,v3,v4	三角形の3頂点
 */
Vector3 Nearest(const Vector3& senterPos, const Vector3& v1, const Vector3& v2, const Vector3& v3);

/*
 *	定数ベクトル
 */
extern const Vector3 V_ZERO;		  // {  0.0f, 0.0f, 0.0f }
extern const Vector3 V_ONE;			  // {  1.0f, 1.0f, 1.0f }
extern const Vector3 V_UP;			  // {  0.0f, 1.0f, 0.0f }
extern const Vector3 V_DOWN;		  // {  0.0f,-1.0f, 0.0f }
extern const Vector3 V_RIGHT;		  // {  1.0f, 0.0f, 0.0f }
extern const Vector3 V_LEFT;		  // { -1.0f, 0.0f, 0.0f }
extern const Vector3 V_FORWARD;		  // {  0.0f, 0.0f, 1.0f }
extern const Vector3 V_BACK;		  // {  0.0f, 0.0f,-1.0f }


/*
 *	Vector3型からVECTOR型への型変換
 *  @param		v	Vector3型
 */
VECTOR ToVECTOR(const Vector3& v);

/*
 *	VECTOR型からVector3型への型変換
 *  @param		v	VECTOR型
 */
Vector3 FromVECTOR(const VECTOR& v);

#endif // !_VECMATH_H_
