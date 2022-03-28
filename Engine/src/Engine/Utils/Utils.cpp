#include "pch.h"

#include "Utils.h"

#include <tinyfiledialogs.h>

namespace tsEngine
{
	namespace Maths
	{
		glm::vec4 U8VecToFloatVec(const glm::u8vec4& u8vec)
		{
			return { float(u8vec.r) / 255.0f, float(u8vec.g) / 255.0f, float(u8vec.b) / 255.0f, float(u8vec.a) / 255.0f };
		}

		glm::u8vec4 FloatVecToU8Vec(const glm::vec4& vec4)
		{
			return { uint8_t(255 * vec4.r), uint8_t(255 * vec4.g), uint8_t(255 * vec4.b), uint8_t(255 * vec4.a) };
		}
	}

	namespace String
	{
		std::string IgnoreCase(const std::string& str)
		{
			std::string out;

			out.reserve(str.size());

			std::transform(str.begin(), str.end(), std::back_inserter(out), ::tolower);

			return out;
		}
	}

	namespace FileDialogs
	{
		std::filesystem::path OpenFileDialog(const std::string& title, const std::string& defaultpath, const char* filter, const char* fileDescription)
		{
			char* file = tinyfd_openFileDialog(title.c_str(), defaultpath.c_str(), filter ? 1 : 0, filter ? &filter : nullptr, fileDescription, 0);

			if (file)
				return std::filesystem::path(file);

			return std::filesystem::path();
		}

		std::filesystem::path SaveFileDialog(const std::string& title, const std::string& defaultpath, const char* filter, const char* fileDescription)
		{
			char* file = tinyfd_saveFileDialog(title.c_str(), defaultpath.c_str(), filter ? 1 : 0, filter ? &filter : nullptr, fileDescription);

			if (file)
				return std::filesystem::path(file);

			return std::filesystem::path();
		}
	}
}