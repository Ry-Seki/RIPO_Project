/*
 *  @file   Title.h
 *  @author Seki
 */

#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

#include "Scene.h"
#include "../GameEnum.h"

/*
 *  タイトルシーン
 */
class TitleScene : public Scene {
private:
    bool isStart = false;
    bool inputHandle = false;
    GameEnum::GameMode gameMode = GameEnum::GameMode::Invalid;

public:
    /*
     *  初期化処理
     */
    virtual void Initialize(Engine& engine) override;
    /*
     *  ロード済みデータのセット (コールバック)
     */
    void SetupData(Engine& engine);
    /*
     *  @brief  準備前処理
     */
    void Setup(Engine& engine) override;
    /*
     *  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    virtual void Render() override;

};
#endif // !_TITLESCENE_H_
