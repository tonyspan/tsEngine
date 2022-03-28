#pragma once

#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Editor.h"

class EditorApp : public tsEngine::Application
{
public:
    EditorApp() = default;
    virtual ~EditorApp() = default;
protected:
    void ClientDefWindowData() override;
    bool ClientDefInit() override;
    void ClientDefOnUpdate(float ts) override;
    bool ClientDefShutdown() override;
    void ClientDefOnEvent(tsEngine::Event& event) override;
    void ClientDefOnImGuiRender() override;
private:
    void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
    void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
    void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);
private:
    Editor m_Editor;
};
