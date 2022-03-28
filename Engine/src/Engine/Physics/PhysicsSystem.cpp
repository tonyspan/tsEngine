#include "pch.h"

#include "PhysicsSystem.h"

#include "Engine/Log/Log.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

// Ref: https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection

namespace tsEngine
{
	PhysicsSystem::PhysicsSystem()
	{
		LOG_INFO("Initializing PhysicsSystem");
	}

	PhysicsSystem::~PhysicsSystem()
	{
		LOG_INFO("Shutting down PhysicsSystem");
	}

	void PhysicsSystem::OnUpdate(float ts, const Ref<EntityManager>& entityManager)
	{
		auto view = entityManager->GetAllEntitiesWith<TransformComponent, MoverComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& mover = view.get<MoverComponent>(entity);

			transform.Position += mover.TranslationSpeed * ts;
			transform.Rotation += mover.RotationSpeed * ts;
		}

		auto view2 = entityManager->GetAllEntitiesWith<TransformComponent, CollisionComponent>();

		for (auto entity : view2)
		{
			view2.get<CollisionComponent>(entity).CollidedWith.clear();
		}

		for (auto entity1 : view2)
		{
			for (auto entity2 : view2)
			{
				if (entity1 == entity2)
					break;

				bool collided = CheckForCollision(entityManager, entity1, entity2);

				if (collided)
				{
					view2.get<CollisionComponent>(entity1).CollidedWith.insert(entity2);
				}
			}
		}
	}

	bool PhysicsSystem::CheckForCollision(const Ref<EntityManager>& entityManager, const entt::entity entity1, const entt::entity entity2)
	{
		auto& transform1 = entityManager->GetComponent<TransformComponent>(entity1);
		auto& collider1 = entityManager->GetComponent<CollisionComponent>(entity1);

		auto& transform2 = entityManager->GetComponent<TransformComponent>(entity2);
		auto& collider2 = entityManager->GetComponent<CollisionComponent>(entity2);

		if (collider1.Shape == CollisionShape::Circle && collider2.Shape == CollisionShape::Circle)
		{
			auto center1 = transform1.Position + transform1.Size / 2.0f;
			auto center2 = transform2.Position + transform2.Size / 2.0f;

			auto sizeSumSqrd = collider1.Size.x * collider1.Size.x + collider2.Size.x * collider2.Size.x;
			auto distanceSqrd = glm::distance(center1, center2);

			return sizeSumSqrd >= distanceSqrd;
		}
		else if (collider1.Shape == CollisionShape::AABox && collider2.Shape == CollisionShape::AABox)
		{
			bool collisionX = transform1.Position.x + collider1.Size.x / 2.0f >= transform2.Position.x - collider2.Size.x / 2.0f &&
				transform2.Position.x + collider2.Size.x / 2.0f >= transform1.Position.x - collider1.Size.x / 2.0f;

			bool collisionY = transform1.Position.y + collider1.Size.y / 2.0f >= transform2.Position.y - collider2.Size.y / 2.0f &&
				transform2.Position.y + collider2.Size.y / 2.0f >= transform1.Position.y - collider1.Size.y / 2.0f;

			return collisionX && collisionY;
		}
		else
		{
			if (collider1.Shape == CollisionShape::Circle && collider2.Shape == CollisionShape::AABox)
			{
				return CheckForAABoxAndCircle(entityManager, entity2, entity1);
			}
			else if (collider1.Shape == CollisionShape::AABox && collider2.Shape == CollisionShape::Circle)
			{
				return CheckForAABoxAndCircle(entityManager, entity1, entity2);
			}

		}

		ASSERT(false, "Unknown collision shapes");

		return false;
	}

	bool PhysicsSystem::CheckForAABoxAndCircle(const Ref<EntityManager>& entityManager, const entt::entity aabox, const entt::entity circle)
	{
		auto& transform1 = entityManager->GetComponent<TransformComponent>(aabox);
		auto& collider1 = entityManager->GetComponent<CollisionComponent>(aabox);

		auto& transform2 = entityManager->GetComponent<TransformComponent>(circle);
		auto& collider2 = entityManager->GetComponent<CollisionComponent>(circle);

		glm::vec2 aabbHalfSize = collider1.Size / 2.0f;
		glm::vec2 aabbCenter = transform1.Position + aabbHalfSize;

		glm::vec2 circleCenter = transform2.Position + collider2.Size.x;

		glm::vec2 centerDistance = circleCenter - aabbCenter;
		glm::vec2 clampedDistance = glm::clamp(centerDistance, -aabbHalfSize, aabbHalfSize);

		glm::vec2 closest = aabbCenter + clampedDistance;

		centerDistance = closest - circleCenter;

		return glm::length(centerDistance) < transform2.Size.x;
	}

}