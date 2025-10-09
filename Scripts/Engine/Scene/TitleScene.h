#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_
#include "Scene.h"

class TitleScene : public Scene {
private:

public:
    virtual void Initialize(Engine& engine) override;
    virtual void Update(Engine& engine, float deltaTime) override;
    virtual void Render() override;

};
#endif // !_TITLESCENE_H_
