#include "Engine.h"

#include "Constants.h"

// Technically is a rect with circle texture :)
class RedCircle : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
        AddComponent<tsEngine::TransformComponent>(-480.0f, -220.0f, 300.0f, 250.0f);
        AddComponent<tsEngine::MoverComponent>();
        AddComponent<tsEngine::SpriteComponent>();
        GetComponent<tsEngine::SpriteComponent>().Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("redcircle");
    }

    virtual void OnUpdate(float ts) override
    {
    }

    virtual void OnDestroy() override
    {
    }
};