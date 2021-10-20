#pragma once

#include "Engine.h"

class ContentPanel
{
public:
	ContentPanel();

	void OnImGuiRender();
private:
	std::filesystem::path m_CurrentDirectory;
};