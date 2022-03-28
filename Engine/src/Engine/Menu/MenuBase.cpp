#include "pch.h"

#include "MenuBase.h"

#include "Engine/Core/Application.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/Render/RenderManager.h"
#include "Engine/Audio/AudioManager.h"

#include "Engine/Events/Events.h"

#include "Engine/Render/Renderer.h"
#include "Engine/Render/Texture.h"

#include <SDL.h>

namespace tsEngine
{
	void MenuBase::CreateMenu(const Ref<Texture>& texture)
	{
		m_MenuPosition = { s_StartingOffset - s_Offset, 0.0f };
		s_Offset += Application::Get().m_Window->GetWidth();

		auto menuEntity = Application::Get().m_EntityManager->CreateEntity(m_MenuName);
		Application::Get().m_EntityManager->AddComponent<TransformComponent>(menuEntity, m_MenuPosition);
		Application::Get().m_EntityManager->AddComponent<SpriteComponent>(menuEntity);

		if (texture)
		{
			Application::Get().m_EntityManager->GetComponent<SpriteComponent>(menuEntity).Image = texture;
			auto& tc = Application::Get().m_EntityManager->GetComponent<TransformComponent>(menuEntity);
			tc.Size = texture->Size();
			s_Offset += texture->Size().x;
		}
	}

	void MenuBase::ShowMenu()
	{
		auto e = Application::Get().m_EntityManager->FindEntityByTag(m_MenuName);
		auto& tc = Application::Get(). m_EntityManager->GetComponent<TransformComponent>(e);

		auto& camera = Application::Get().m_RenderManager->GetCamera();

		camera.Transform->Position = tc.Position;
		//camera.Transform->Size = tc.Size;
	}

	const glm::vec2& MenuBase::MenuGetPosition() const
	{
		return m_MenuPosition;
	}

	bool MenuBase::MenuEntityHovered(const std::string& entityTag, const glm::vec2& mousePos)
	{
		auto view = Application::Get().m_EntityManager->GetAllEntitiesWith<TagComponent, TransformComponent, TextComponent>();

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
		const auto& camera = Application::Get().m_RenderManager->GetCamera();

		auto& transform = Application::Get().m_EntityManager->GetComponent<TransformComponent>((entt::entity)entity);

		auto screenPosition = tsEngine::Renderer::GetScreenPosition(transform.Position, camera);

		SDL_Rect dst = { (int)(screenPosition.x - transform.Size.x / 2), (int)(screenPosition.y - transform.Size.y / 2), (int)transform.Size.x, (int)transform.Size.y };

		SDL_Point point = { (int)mousePos.x, (int)mousePos.y };

		if (SDL_PointInRect(&point, &dst))
		{
			return true;
		}

		return false;
	}
}