/*
 *  @file   Title.h
 *  @author Seki
 */

#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

#include "Scene.h"

/*
 *  タイトルシーン
 */
class TitleScene : public Scene {
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
     *  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    virtual void Render() override;

};
#endif // !_TITLESCENE_H_
