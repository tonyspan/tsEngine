#pragma once

#include "Engine.h"

class BasicPhysics : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
        AddComponent<tsEngine::CollisionComponent>();
        
        if (HasComponent<tsEngine::SpriteComponent>())
        {
            GetComponent<tsEngine::CollisionComponent>().Shape = tsEngine::CollisionShape::AABox;
            AddComponent<tsEngine::MoverComponent>(m_Speed, 0.0f, 0.0f);
        }
        else
        {
            AddComponent<tsEngine::MoverComponent>(-m_Speed, 0.0f, 0.0f);
        }

    }

    virtual void OnUpdate(float ts) override
    {
        auto& cc = GetComponent<tsEngine::CollisionComponent>();
        auto& mc = GetComponent<tsEngine::MoverComponent>();
        
        for (auto e : cc.CollidedWith)
        {
            // Other entity's component
            auto& omc = GetComponent<tsEngine::MoverComponent>(e);
            omc.TranslationSpeed.x = -omc.TranslationSpeed.x;
            
            mc.TranslationSpeed.x = -mc.TranslationSpeed.x;
        }

    }

    virtual void OnDestroy() override
    {
    }

private:
    float m_Speed = 10.0f;
};