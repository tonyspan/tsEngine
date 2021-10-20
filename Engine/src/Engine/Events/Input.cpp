#include "pch.h"

#include "Input.h"

namespace tsEngine
{
	MouseButtonEvent s_MouseEvent = MouseButtonEvent(false, 0, 0, false);
	KeyboardEvent s_KeyboardEvent = KeyboardEvent(false, 0, KeyCode::None);
	MousePositionEvent s_MousePositionEvent = MousePositionEvent({ 0, 0 }, { 0, 0 });
	KeyboardTypedEvent s_KeyboardTypedEvent = KeyboardTypedEvent(" ");
	MouseWheelEvent s_Wheel = MouseWheelEvent(0);

	bool Input::MouseButtonDown() { return s_MouseEvent.Down; }
	bool Input::MouseButtonClicked() { return s_MouseEvent.Clicked; }
	glm::vec2 Input::MousePosition() { return s_MousePositionEvent.Position; }
	MouseButtonId Input::MouseButtonId() { return s_MouseEvent.ButtonId; }

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);
		return (state[(SDL_Scancode)keyCode]);
	}

	std::string& Input::UserInput() { return s_KeyboardTypedEvent.InputText; }

	glm::vec2 Input::MouseMotion() { return s_MousePositionEvent.Motion; }
	int Input::MouseWheelDirection() { return s_Wheel.Direction; }
	void Input::ResetWheel() { s_Wheel.Direction = 0; }
}