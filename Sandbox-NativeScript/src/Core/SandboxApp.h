#pragma once

#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

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
    tsEngine::Ref<tsEngine::EntityManager> m_EntityManager;
};
