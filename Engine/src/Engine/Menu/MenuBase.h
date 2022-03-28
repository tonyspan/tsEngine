#pragma once

#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	struct Event;
	class Texture;

	// NOTE: Defenitely not the proper way to create menu screens
	class MenuBase
	{
	public:
		MenuBase(const std::string& name = "MenuBase")
			: m_MenuName(name)
		{
		}

		virtual ~MenuBase() = default;

		virtual void OnCreate(const Ref<Texture>& texture = nullptr) = 0;
		virtual void OnUpdate(float ts) = 0;
		virtual void OnEvent(Event& event) = 0;

		const std::string& GetName() const { return m_MenuName; }
	protected:
		// Creates a menu screen
		void CreateMenu(const Ref<Texture>& texture = nullptr);

		void ShowMenu();

		// Returns the "world" position of the menu screen
		const glm::vec2& MenuGetPosition() const;
		
		bool MenuEntityHovered(const std::string& entityTag, const glm::vec2& mousePos);
	protected:
		std::string m_MenuName;
	private:
		bool IsEntityHovered(uint32_t entity, const glm::vec2& mousePos);
	private:
		glm::vec2 m_MenuPosition{};

		static inline float s_StartingOffset = -5000.0f;
		static inline float s_Offset = 0.0f;
	};
}