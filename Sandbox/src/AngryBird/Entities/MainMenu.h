#pragma once

#include "Engine.h"

namespace Game
{
	// See base class for more info
	class MainMenu : public tsEngine::MenuBase
	{
	public:
		MainMenu();
		virtual void OnCreate(const tsEngine::Ref<tsEngine::Texture>& texture) override;
		virtual void OnUpdate(float ts) override;
		virtual void OnEvent(tsEngine::Event& event) override;
	private:
		void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
		void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
		void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);
	};
}