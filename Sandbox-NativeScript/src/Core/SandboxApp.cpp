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
    tsEngine::RenderSystem::Get().GetRenderer()->SetBackgroundColor({ 0, 0, 0, 1 });

    auto tex1 = tsEngine::Asset::Create<tsEngine::Texture>(Constants::BG);
    auto tex2 = tsEngine::Asset::Create<tsEngine::Texture>(Constants::GREEN);
    auto tex3 = tsEngine::Asset::Create<tsEngine::Texture>(Constants::RED);

    tsEngine::AssetManager::AddAsset<tsEngine::Texture>("checkerboard", tex1);
    tsEngine::AssetManager::AddAsset<tsEngine::Texture>("greensquare", tex2);
    tsEngine::AssetManager::AddAsset<tsEngine::Texture>("redcircle", tex3);

    m_EntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
    tsEngine::EntityManager::SetActive(m_EntityManager);

    auto camera = m_EntityManager->CreateEntity("MainCamera");
    m_EntityManager->AddComponent<tsEngine::NativeScriptComponent>(camera).Bind<CameraController>();

    auto checkerboard = m_EntityManager->CreateEntity();
    m_EntityManager->AddComponent<tsEngine::NativeScriptComponent>(checkerboard).Bind<Checkerboard>();

    auto greenSquare = m_EntityManager->CreateEntity();
    m_EntityManager->AddComponent<tsEngine::NativeScriptComponent>(greenSquare).Bind<GreenSquare>();

    auto redCircle = m_EntityManager->CreateEntity();
    m_EntityManager->AddComponent<tsEngine::NativeScriptComponent>(redCircle).Bind<RedCircle>();

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
