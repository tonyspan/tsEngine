#pragma once

#include "Engine.h"

#include <SDL.h>

namespace Game
{
	// See base class for more info
	class MainMenu : public tsEngine::MenuBase
	{
	public:
		virtual void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texture) override;
		virtual void OnUpdate(float ts, tsEngine::CameraData& camera, const tsEngine::Ref<tsEngine::AudioManager>& audioManager) override;
		void OnEvent(tsEngine::MouseButtonEvent& event);
		static bool s_HideMenu;
	};
}