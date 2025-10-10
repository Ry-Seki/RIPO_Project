#ifndef _TIME_H_
#define _TIME_H_

#include <chrono>

class Time {
public:
    static float deltaTime;        // スケール付き経過時間
    static float unscaledDeltaTime; // スケールなし
    static float timeScale;        // ゲーム全体の時間倍率
    static float totalTime;        // 累計起動時間（スケール考慮）

private:
    static std::chrono::high_resolution_clock::time_point previousTime;

public:
    static void Init() {
        previousTime = std::chrono::high_resolution_clock::now();
        deltaTime = 0.0f;
        unscaledDeltaTime = 0.0f;
        totalTime = 0.0f;
        timeScale = 1.0f;
    }

    static void Update() {
        // 現在時間の取得
        auto currentTime = std::chrono::high_resolution_clock::now();
        // 前フレームとの差分を求める
        std::chrono::duration<float> delta = currentTime - previousTime;
        // スケールに影響なしのdeltaTime
        unscaledDeltaTime = delta.count();
        // スケールに影響ありのdeltaTime
        deltaTime = unscaledDeltaTime * timeScale;
        // 累計時間の加算
        totalTime += deltaTime;
        // 現在時間を前フレーム時間に代入
        previousTime = currentTime;
    }
}; 
#endif // !_TIME_H_