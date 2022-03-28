#pragma once

#include "pch.h"

namespace tsEngine
{
	struct Event
	{
	public:
		Event(const std::string& name)
			: Active(true), Name(name)
		{
		}

		virtual ~Event() = default;

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
		}

		virtual ~EventDispatcher()
		{
			m_Event = nullptr;
		}

		template<typename T>
		bool Dispatch(const EventFn<T> func)
		{
			if (dynamic_cast<T*>(m_Event))
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