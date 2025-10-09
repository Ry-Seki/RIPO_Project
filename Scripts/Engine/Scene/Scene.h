#ifndef _SCENE_H_
#define _SCENE_H_

#include "../GameObject.h"
#include <vector>
#include <memory>

// ‘O•ûéŒ¾
class Engine;

class Scene {
protected:
    std::vector<GameObjectPtr> gameObjects;

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Initialize(Engine& engine);
    virtual void Update(Engine& engine, float deltaTime);
    virtual void Render();
    virtual void Finalize(Engine& engine);

public:
    // GameObject ŠÇ—
    void AddGameObject(const GameObjectPtr& obj);
    void RemoveDestroyedObjects();
};
using ScenePtr = std::shared_ptr<Scene>;
#endif // !_SCENE_H_