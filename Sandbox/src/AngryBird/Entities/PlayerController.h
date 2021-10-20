#pragma once

#include "Engine.h"

namespace Game
{
    class PlayerController
    {
    public:
        void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texture);
        void OnUpdate(float ts, const tsEngine::Ref<tsEngine::EntityManager>& entityManager);
        void OnEvent(tsEngine::Event& event);
        
        const int GetPlayerScore() const;
    public:
        static bool s_StillPlaying;
        static int s_Score;
    private:
        void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
        void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
        void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);
    };
}