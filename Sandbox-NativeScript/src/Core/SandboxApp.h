#pragma once

#include <Engine/Core/Application.h>

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
};

tsEngine::Application* tsEngine::CreateApplication()
{
    return new Sandbox();
}
