#pragma once

#include "pch.h"

#include "PlatformDetection.h"

namespace tsEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	// Custom deleter for unique_ptr
	// Unused
	template<auto FUNC, typename T>
	struct Deleter
	{
		void operator()(T* ptr)
		{
			FUNC(ptr);
		}
	};

	// Custom deleter for unique_ptr
	template<typename T>
	struct Deleter2
	{
		void operator()(T* ptr);
	};

	template<typename T>
	using Scope2 = std::unique_ptr<T, Deleter2<T>>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#define ENGINE_BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
