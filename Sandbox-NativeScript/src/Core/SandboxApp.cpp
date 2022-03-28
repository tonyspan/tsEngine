#include "SandboxApp.h"

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
    m_RenderManager->GetRenderer()->SetBackgroundColor({ 0, 0, 0, 1 });

    auto tex1 = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Constants::BG);
    auto tex2 = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Constants::GREEN);
    auto tex3 = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Constants::RED);

    tsEngine::AssetManager::AddTexture("checkerboard", tex1);
    tsEngine::AssetManager::AddTexture("greensquare", tex2);
    tsEngine::AssetManager::AddTexture("redcircle", tex3);

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
}

tsEngine::Application* tsEngine::CreateApplication()
{
    return new Sandbox();
}
