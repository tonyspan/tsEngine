#pragma once

#include "pch.h"
#include "Engine/Log/Log.h"

#include <SDL.h>

namespace tsEngine
{
	struct Event
	{
	public:
		Event(const std::string& name)
			: Active(true), Name(name)
		{
			SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
		}

		virtual ~Event() = default;

		inline bool IsActive() const { return Active; }
		inline void SetActive(bool active) { Active = active; }

		virtual std::string GetName() const { return ""; }
	protected:
		bool Active;
		std::string Name;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<void(T&)>;
	public:
		EventDispatcher(Event* event)
			: m_Event(event)
		{
			//LOG_INFO("Event: {0}", event->GetName());
		}

		virtual ~EventDispatcher()
		{
			m_Event = nullptr;
		}

		template<typename T>
		bool Dispatch(const EventFn<T> func)
		{
			if (m_Event->IsActive() && dynamic_cast<T*>(m_Event))
			{
				func(static_cast<T&>(*m_Event));
				return true;
			}
			return false;
		}

		inline Event* GetEvent() { return m_Event; }
	private:
		Event* m_Event;
	};
}