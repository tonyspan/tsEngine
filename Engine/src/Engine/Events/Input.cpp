#include "pch.h"

#include "Input.h"

#include "Events.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"

namespace tsEngine
{
	MouseButtonEvent g_MouseEvent = MouseButtonEvent(false, 0, 0, false);
	KeyboardEvent g_KeyboardEvent = KeyboardEvent(false, 0, KeyCode::None);
	MousePositionEvent g_MousePositionEvent = MousePositionEvent({ 0, 0 }, { 0, 0 });
	KeyboardTypedEvent g_KeyboardTypedEvent = KeyboardTypedEvent(" ");
	MouseWheelEvent g_Wheel = MouseWheelEvent(0);

	bool Input::IsMousePressed(MouseCode mouseButton)
	{
		const auto state = SDL_GetMouseState(NULL, NULL);
		return state == SDL_BUTTON((int)mouseButton);
	}

	bool Input::MouseButtonDown() { return g_MouseEvent.Down; }
	bool Input::MouseButtonClicked() { return g_MouseEvent.Clicked; }
	glm::vec2& Input::MousePosition() { return g_MousePositionEvent.Position; }

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);
		return state[(SDL_Scancode)keyCode];
	}

	std::string& Input::UserInput() { return g_KeyboardTypedEvent.InputText; }

	glm::vec2& Input::MouseMotion() { return g_MousePositionEvent.Motion; }
	int Input::MouseWheelDirection() { return g_Wheel.Direction; }
	void Input::ResetWheel() { g_Wheel.Direction = 0; }

	const char* Input::GetName(KeyCode keyCode) { return KeyCodeName(keyCode); }
	const char* Input::GetName(MouseCode mouseCode) { return MouseCodeName(mouseCode); }
}