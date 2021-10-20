#include "SandboxApp.h"

#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "../Scripts/CameraController.h"
#include "../Scripts/Checkerboard.h"
#include "../Scripts/GreenSquare.h"
#include "../Scripts/RedCircle.h"
#include "../Scripts/Constants.h"

void Sandbox::ClientDefWindowData()
{
    m_WindowData.Title = Constants::TITLE;
}

bool Sandbox::ClientDefInit()
{
    m_RenderManager->SetBackgroundColor({ 0, 0, 0, 1 });

    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "checkerboard", Constants::BG);
    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "greensquare", Constants::GREEN);
    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "redcircle", Constants::RED);

    auto camera = m_EntityManager->CreateEntity("MainCamera");
    m_EntityManager->AddNativeScript<tsEngine::NativeScriptComponent>(camera).Bind<CameraController>();

    auto checkerboard = m_EntityManager->CreateEntity();
    m_EntityManager->AddNativeScript<tsEngine::NativeScriptComponent>(checkerboard).Bind<Checkerboard>();

    auto greenSquare = m_EntityManager->CreateEntity();
    m_EntityManager->AddNativeScript<tsEngine::NativeScriptComponent>(greenSquare).Bind<GreenSquare>();

    auto redCircle = m_EntityManager->CreateEntity();
    m_EntityManager->AddNativeScript<tsEngine::NativeScriptComponent>(redCircle).Bind<RedCircle>();

    LOG_WARN("Press WASD keys to move the camera");

    return true;
}

void Sandbox::ClientDefOnUpdate(float ts)
{
}

bool Sandbox::ClientDefShutdown()
{
    return true;
}

void Sandbox::ClientDefOnEvent(tsEngine::Event& event)
{
    tsEngine::EventDispatcher dispatcher(&event);

    dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
    dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
    dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
}

void Sandbox::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
{
}

void Sandbox::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
{
}

void Sandbox::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
{
}
