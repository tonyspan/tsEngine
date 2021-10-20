#pragma once

#include "pch.h"

#include <glm/glm.hpp>

#include "imgui.h"

namespace tsEngine
{
	namespace Maths
	{
		static constexpr glm::vec4 U8VecToFloatVec(const glm::u8vec4& u8vec)
		{
			return { float(u8vec.r) / 255.0f, float(u8vec.g) / 255.0f, float(u8vec.b) / 255.0f, float(u8vec.a) / 255.0f };
		}

		static constexpr glm::u8vec4 FloatVecToU8Vec(const glm::vec4& vec4)
		{
			return { uint8_t(255 * vec4.r), uint8_t(255 * vec4.g), uint8_t(255 * vec4.b), uint8_t(255 * vec4.a) };
		}
	}

	namespace String
	{
		static auto IgnoreCase(const std::string& str)
		{
			std::string out;

			out.reserve(str.size());

			std::transform(str.begin(), str.end(), std::back_inserter(out), ::tolower);

			return out;
		}
	}

	static void ImGuiHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}