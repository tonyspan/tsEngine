#include "ContentPanel.h"

// BasePath for assets/content has trailing slash, parent_path() removes it
const auto g_Path = tsEngine::AssetManager::s_BasePath.parent_path();

// Not show the source code folder
const auto g_Core = "Core";
const auto g_CorePathString = std::filesystem::path(g_Core).string();

ContentPanel::ContentPanel()
	: m_CurrentDirectory(g_Path)
{
}

void ContentPanel::OnImGuiRender()
{
	bool active = true;
	const ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove /*| ImGuiWindowFlags_NoBackground*/ | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

	static std::string currPathString = g_Path.string();
	
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	static std::string toBeSearched;

	std::string contentBrowser = "Content Browser";

	ImGui::Begin(contentBrowser.c_str(), &active, winFlags);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	ImGui::LabelText("", contentBrowser.c_str());
	ImGui::SameLine(contentBrowser.size() + 120.0f);
	
	ImGui::PushItemWidth(150.0f);
	if (ImGui::InputTextWithHint("##search", "Search...", buffer, IM_ARRAYSIZE(buffer)))
		toBeSearched = std::string(buffer);
	
	ImGui::SameLine();
	ImGui::PushItemWidth(500.0f);
	ImGui::LabelText("", currPathString.c_str());
	
	ImGui::Separator();

	if (m_CurrentDirectory != g_Path)
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();

			currPathString = m_CurrentDirectory.string();
		}
	}

	static float padding = 8.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	if (toBeSearched.empty())
	{
		for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_Path);
			std::string filenameString = relativePath.filename().string();

			if (filenameString != g_CorePathString)
			{
				ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize });

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();

					currPathString = m_CurrentDirectory.string();
				}
				//ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();
			}
		}
	}
	else
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(g_Path))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_Path);
			std::string filenameString = relativePath.filename().string();

			if (relativePath.parent_path().string() != g_CorePathString)
			{
				toBeSearched = tsEngine::String::IgnoreCase(toBeSearched);
				auto loweredFilenameString = tsEngine::String::IgnoreCase(filenameString);
				
				if (toBeSearched == loweredFilenameString)
				{
					ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize });

					if (ImGui::BeginDragDropSource())
					{
						const wchar_t* itemPath = relativePath.c_str();
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						ImGui::EndDragDropSource();
					}
				}
			}
		}

		toBeSearched.clear();
	}

	ImGui::Columns(1);

	ImGui::PopStyleVar();

	ImGui::End();
}