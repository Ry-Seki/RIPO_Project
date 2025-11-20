/*
 *	@file	Time.cpp
 *	@author	Seki
 */

#include "Time.h"

// Ã“Iƒƒ“ƒo‚ÌÀ‘Ì’è‹`
float Time::deltaTime = 0.0f;
float Time::unscaledDeltaTime = 0.0f;
float Time::timeScale = 1.0f;
float Time::totalTime = 0.0f;
float Time::fps = 0.0f;
std::chrono::high_resolution_clock::time_point Time::previousTime;