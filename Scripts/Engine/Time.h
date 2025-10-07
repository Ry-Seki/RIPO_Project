/*
 *	@file	Time.h
 */

#ifndef _TIME_H_
#define _TIME_H_

#include <chrono>

class Time {
public:
    static float deltaTime;       // スケール付き経過時間（今はスケール未使用）
    static float unscaledDeltaTime; // スケールなし
private:
    static std::chrono::high_resolution_clock::time_point previousTime;

public:
    // 初期化（Engine::Initialize() 内で呼ぶ）
    static void Init() {
        previousTime = std::chrono::high_resolution_clock::now();
        deltaTime = 0.0f;
        unscaledDeltaTime = 0.0f;
    }

    // 毎フレーム更新（Engine::Update() 内で呼ぶ）
    static void Update() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> diff = currentTime - previousTime;
        deltaTime = diff.count();
        unscaledDeltaTime = deltaTime;
        previousTime = currentTime;
    }
}; 
#endif // !_TIME_H_
