#include "EditorApp.h"

#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

void EditorApp::ClientDefWindowData()
{
    m_WindowData.Title = "EngineEd";
    m_WindowData.VSync = true;
}

bool EditorApp::ClientDefInit()
{
    m_RenderManager->SetBackgroundColor({ 0, 0, 0, 1 });

    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), tsEngine::AssetManager::s_DefaultTexturePath.string());

    m_Editor.OnCreate(m_EntityManager, m_RenderManager);

    return true;
}

void EditorApp::ClientDefOnUpdate(float ts)
{
    m_Editor.OnUpdate();
}

bool EditorApp::ClientDefShutdown()
{
    return true;
}

void EditorApp::ClientDefOnEvent(tsEngine::Event& event)
{
    tsEngine::EventDispatcher dispatcher(&event);

    dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
    dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
    dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
}

void EditorApp::ClientDefOnImGuiRender()
{
    m_Editor.OnImGuiRender();
}

void EditorApp::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
{
    m_Editor.OnEvent(event);
}

void EditorApp::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
{
    m_Editor.OnEvent(event);
}

void EditorApp::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
{
    m_Editor.OnEvent(event);
}
