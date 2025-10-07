#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::unscaledDeltaTime = 0.0f;
std::chrono::high_resolution_clock::time_point Time::previousTime;