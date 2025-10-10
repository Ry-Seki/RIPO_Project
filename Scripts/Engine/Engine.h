#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "GameObject.h"
#include "Scene/Scene.h"
#include "VecMath.h"
#include "Fade/FadeBase.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>

class Engine {
private:
    ScenePtr currentScene;
    ScenePtr nextScene;

    bool dxlibInitialized = false;
    bool effekseerInitialized = false;
    bool initialized = false;

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

public:
    Engine() = default;
    ~Engine() = default;

    int Run();

private:
    int Initialize();
    void Teardown();
    void Update();
    void Render();

public:
    void SetNextScene(ScenePtr setScene) { nextScene = setScene; }
    void ChangeScene();
    void StartSceneFade(const FadeBasePtr& setFade, std::function<void()> onComplete = nullptr);

    // GameObjectê∂ê¨
    template<class T>
    std::shared_ptr<T> Create(const std::string& name = "",
                              const Vector3& position = Vector3::zero,
                              const Vector3& rotation = Vector3::zero) {
        auto obj = std::make_shared<T>();
        obj->engine = this;
        obj->name = name;
        obj->position = position;
        obj->rotation = rotation;

        if (currentScene) currentScene->AddGameObject(obj);
        return obj;
    }
};

#endif // !_ENGINE_H_

