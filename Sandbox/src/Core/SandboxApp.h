#pragma once

#include <Engine/Core/Application.h>

#include "../AngryBird/Entities/Constants.h"
#include "../AngryBird/Entities/CameraController.h"
#include "../AngryBird/Entities/PlayerController.h"
#include "../AngryBird/Entities/Level.h"
#include "../AngryBird/Entities/ObstacleController.h"
#include "../AngryBird/Entities/MainMenu.h"
#include "../AngryBird//Entities/EndMenu.h"

class Sandbox : public tsEngine::Application
{
public:
    Sandbox() = default;
    virtual ~Sandbox() = default;
protected:
    void ClientDefWindowData() override;
    bool ClientDefInit() override;
    void ClientDefOnUpdate(float ts) override;
    bool ClientDefShutdown() override;
    void ClientDefOnEvent(tsEngine::Event& event) override;
private:
    void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
    void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
    void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);
private:
    Game::PlayerController m_PlayerController;
    Game::CameraController m_CameraController;
    Game::ObstacleController m_ObstacleController;
    Game::Level m_Level;
    Game::MainMenu m_MainMenu;
    Game::EndMenu m_EndMenu;
};

tsEngine::Application* tsEngine::CreateApplication()
{
    return new Sandbox();
}
