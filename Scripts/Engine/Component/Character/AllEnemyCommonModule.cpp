/*
 *	@file	AllEnemyCommonModule.cpp
 *	@author	kuu
 */
#include "AllEnemyCommonModule.h"

float EnemyNormalizeAngle(float angle)
{
	while (angle > Pi) angle -= 2.0f * Pi;
	while (angle < -Pi) angle += 2.0f * Pi;
	return angle;
}
