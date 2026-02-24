/*
 *	@file	AllEnemyCommonModule.cpp
 *	@author	kuu
 */
#include "AllEnemyCommonModule.h"

 /*
  *	Šp“x·‚ð³‹K‰»
  */
float EnemyNormalizeAngle(float angle)
{
	while (angle > Pi) angle -= 2.0f * Pi;
	while (angle < -Pi) angle += 2.0f * Pi;
	return angle;
}
