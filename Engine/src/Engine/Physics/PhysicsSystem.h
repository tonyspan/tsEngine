#pragma once

#include "Engine/Core/Base.h"

#include "entt.hpp"

namespace tsEngine
{
    class EntityManager;

    class PhysicsSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        void OnUpdate(float ts, const Ref<EntityManager>& entityManager);
    private:
        bool CheckForCollision(const Ref<EntityManager>& entityManager, const entt::entity entity1, const entt::entity entity2);
        bool CheckForAABoxAndCircle(const Ref<EntityManager>& entityManager, const entt::entity aabox, const entt::entity circle);
    };
}