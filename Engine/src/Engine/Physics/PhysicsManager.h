#pragma once

#include "Engine/Core/Base.h"

#include "Engine/ECS/Components.h"

namespace tsEngine
{
    class EntityManager;

    struct EntityData
    {
        TransformComponent* tc = nullptr;
        CollisionComponent* cc = nullptr;
        MoverComponent* mc = nullptr;
    };

    class PhysicsManager
    {
    public:
        PhysicsManager() = default;
        ~PhysicsManager() = default;

        void OnUpdate(float ts, const Ref<EntityManager>& entityManager);
    private:
        bool CheckForCollision(const EntityData& entity1, const EntityData& entity2);
        bool CheckForAABoxAndCircle(const EntityData& entity1, const EntityData& entity2);
    };
}