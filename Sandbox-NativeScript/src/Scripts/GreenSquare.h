#include "Engine.h"

#include "Constants.h"

class GreenSquare : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
        AddComponent<tsEngine::TransformComponent>(100.0f, 100.0f, 200.0f, 200.0f);
        GetComponent<tsEngine::TransformComponent>().Rotation = 45.0f;
        AddComponent<tsEngine::SpriteComponent>();
        GetComponent<tsEngine::SpriteComponent>().Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("greensquare");
    }

    virtual void OnUpdate(float ts) override
    {
        GetComponent<tsEngine::TransformComponent>().Rotation += 50.0f * ts;
    }

    virtual void OnDestroy() override
    {
    }
};