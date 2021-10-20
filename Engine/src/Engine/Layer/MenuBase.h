#pragma once

#include "Engine/Audio/AudioManager.h"

#include "Engine/ECS/EntityManager.h"

//#include "Engine/Utils/AssetManager.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	// NOTE: Not the proper way to create menu screens?
	// TODO: Offset changes based on DEFAULT_WIDTH and not the actual window width
	class MenuBase
	{
	public:
		virtual ~MenuBase() = default;
	protected:
		virtual void OnCreate(const Ref<EntityManager>& entityManager, Texture* texture) = 0;
		// audioManager needed in case you want to add sfx when selecting option
		virtual void OnUpdate(float ts, CameraData& camera, const Ref<AudioManager>& audioManager) = 0;

		// Creates a menu screen
		void AddMenuScreen(const std::string& menuName)
		{
			auto found = std::find_if(m_MenuScreens.begin(), m_MenuScreens.end(), [&](const auto& pair) { return menuName == pair.first; });

			if (found == m_MenuScreens.end())
			{
				m_MenuScreens.emplace(menuName, glm::vec2{ s_StartingOffset - s_Offset, 0.0f });
				s_Offset += DEFAULT_WIDTH;
			}
		}

		// Returns menu's screen position
		auto& GetMenuScreen(const std::string& menuName) { return m_MenuScreens[menuName]; }
		
		// Returns game's default screen position
		auto& GetDefaultScreen() { return s_OnScreen; }
	private:
		std::unordered_map<std::string, glm::vec2> m_MenuScreens;
		
		static constexpr float s_StartingOffset = -5000.0f;
		
		static inline float s_Offset = 0.0f;
		static inline const glm::vec2 s_OnScreen = { 0.0f, 0.0f };
	};
}