#pragma once

#include "pch.h"

#include "Engine/Core/PlatformDetection.h"

#include "Engine/Log/Log.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
	#ifdef DL_EXPORT
		#define EXPORT_API __declspec(dllexport)
	#else
		#define EXPORT_API __declspec(dllimport)
	#endif
#else
	#define EXPORT_API
#endif

#define ALLOCATOR(X) EXPORT_API X* allocator() { return new X(); }
#define DEALLOCATOR(X) EXPORT_API void deleter(X* ptr) { delete ptr; }

#define EXPORT_CLASS(X) extern "C" { ALLOCATOR(X) DEALLOCATOR(X) }

#ifdef PLATFORM_WINDOWS
#define HANDLE HINSTANCE
#else
#define HANDLE void*
#endif

#ifdef PLATFORM_WINDOWS
#define STDFUNC(X) std::function<Ret(Args...)>(reinterpret_cast<Ret(__stdcall*)(Args...)>(X));
#else
#define STDFUNC(X) std::function<Ret(Args...)>(reinterpret_cast<Ret(*)(Args...)>(X));
#endif

#ifdef PLATFORM_WINDOWS
#define DLOPEN(X) LoadLibrary(X)
#else
#define DLOPEN(X) dlopen(X, RTLD_NOW | RTLD_LAZY)
#endif

#ifdef PLATFORM_WINDOWS
#define DLGET(X, Y) GetProcAddress(X, Y)
#else
#define DLGET(X, Y) dlsym(X, Y)
#endif

#ifdef PLATFORM_WINDOWS
#define DLCLOSE(X) FreeLibrary(X)
#else
#define DLCLOSE(X) dlclose(X)
#endif

namespace tsEngine
{
	class DLLoader
	{
	public:
		static void LoadFile(const std::string& libPath)
		{
			s_Handler = DLOPEN(libPath.c_str());
			
			if (s_Handler == nullptr)
				LOG_INFO("Could not load {} from {}" + std::filesystem::path(libPath).filename().string(), std::filesystem::path(libPath).parent_path().string());
		}

		static void UnloadFile()
		{
			if (s_Handler)
			{
				DLCLOSE(s_Handler);
				s_Handler = nullptr;
			}
		}

		template<typename Ret, typename... Args>
		static std::function<Ret(Args...)> Get(const std::string& funcName)
		{
			auto funcID = DLGET(s_Handler, funcName.c_str());

			if (funcID == nullptr)
				LOG_ERROR("Could not locate symbol {}", funcName);
			
			return STDFUNC(funcID);
		}
	private:
		static inline HANDLE s_Handler = nullptr;
	};
}