#include "pch.h"

#include "MenuBase.h"

#include "Engine/Core/Application.h"

#include "Engine/ECS/EntityManager.h"

#include "Engine/Events/Events.h"

#include "Engine/ECS/Components.h"

#include "Engine/Render/RenderSystem.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Texture.h"

#include <SDL.h>

namespace tsEngine
{
	void MenuBase::CreateMenu(const Ref<Texture>& texture)
	{
		m_MenuPosition = { s_StartingOffset - s_Offset, 0.0f };
		s_Offset += Application::Get().GetWindow()->GetWidth();

		auto entityManager = EntityManager::GetActive();

		auto menuEntity = entityManager->CreateEntity(m_MenuName);
		entityManager->AddComponent<TransformComponent>(menuEntity, m_MenuPosition);
		entityManager->AddComponent<SpriteComponent>(menuEntity);

		if (texture)
		{
			entityManager->GetComponent<SpriteComponent>(menuEntity).Image = texture;
			auto& tc = entityManager->GetComponent<TransformComponent>(menuEntity);
			tc.Size = texture->Size();
			s_Offset += texture->Size().x;
		}
	}

	void MenuBase::ShowMenu()
	{
		auto entityManager = EntityManager::GetActive();

		auto menuEntity = entityManager->FindEntityByTag(m_MenuName);
		auto& menuEntityTc = entityManager->GetComponent<TransformComponent>(menuEntity);

		auto view = entityManager->GetAllEntitiesWith<tsEngine::CameraComponent, tsEngine::TransformComponent>();

		for (auto entity : view)
		{
			auto& tc = view.get<tsEngine::TransformComponent>(entity);

			tc.Position = menuEntityTc.Position;
			//tc.Size = menuEntityTc.Size;
		}
	}

	const glm::vec2& MenuBase::MenuGetPosition() const
	{
		return m_MenuPosition;
	}

	bool MenuBase::MenuEntityHovered(const std::string& entityTag, const glm::vec2& mousePos)
	{
		const auto entityManager = EntityManager::GetActive();

		auto view = entityManager->GetAllEntitiesWith<TagComponent, TransformComponent, TextComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& tag = view.get<TagComponent>(entity);

			if (IsEntityHovered((uint32_t)entity, mousePos) && tag.Tag == entityTag)
			{
				return true;
			}
		}

		return false;
	}

	bool MenuBase::IsEntityHovered(uint32_t entity, const glm::vec2& mousePos)
	{
		const auto& camera = RenderSystem::Get().GetCamera();

		const auto entityManager = EntityManager::GetActive();

		auto& transform = entityManager->GetComponent<TransformComponent>((entt::entity)entity);

		auto screenPosition = Renderer::GetScreenPosition(transform.Position, camera);

		SDL_Rect dst = { (int)(screenPosition.x - transform.Size.x / 2), (int)(screenPosition.y - transform.Size.y / 2), (int)transform.Size.x, (int)transform.Size.y };

		SDL_Point point = { (int)mousePos.x, (int)mousePos.y };

		if (SDL_PointInRect(&point, &dst))
		{
			return true;
		}

		return false;
	}
}