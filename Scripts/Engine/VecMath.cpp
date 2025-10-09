/*
 *	@file	VecMath.cpp
 */

#include "VecMath.h"

// Ã“Iƒƒ“ƒo•Ï”‚ÌéŒ¾
Vector3 Vector3::zero = { 0.0f, 0.0f, 0.0f };
Vector3 Vector3::one = { 1.0f, 1.0f, 1.0f };

Vector3::Vector3() 
	: x(0.0f), y(0.0f), z(0.0f){

}

Vector3::Vector3(float setX, float setY, float setZ)
	: x(setX), y(setY), z(setZ){

}
