#pragma once

#include "Window.h"

//#include <SDL.h>

struct SDL_Window;

namespace tsEngine
{
	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowProps& props);
		~SDLWindow();

		virtual void SetWidth(uint32_t width) override;
		virtual void SetHeight(uint32_t height) override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const override;
	private:
		void Init(const WindowProps& props);
		void AddWindowIcon();
	private:
		Scope2<SDL_Window> m_NativeWindow;
		WindowProps m_Data;
	};
}