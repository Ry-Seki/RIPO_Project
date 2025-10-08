#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_
#include "Scene.h"
#include "../Task.h"
#include "../Engine.h"

class TitleScene : public Scene {
private:
    //std::shared_ptr<TaskVoid> m_debugTask;
    bool requestSceneChange = false;

public:
    virtual bool Initialize(Engine& engine) override;
    virtual void Update(Engine& engine, float deltaTime) override;
    virtual void Render() override;

    TaskVoid DebugTask(Engine& engine);
};
#endif // !_TITLESCENE_H_
