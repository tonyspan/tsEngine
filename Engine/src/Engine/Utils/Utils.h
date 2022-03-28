#pragma once

#include "pch.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	namespace Maths
	{
		glm::vec4 U8VecToFloatVec(const glm::u8vec4& u8vec);

		glm::u8vec4 FloatVecToU8Vec(const glm::vec4& vec4);
	}

	namespace String
	{
		std::string IgnoreCase(const std::string& str);
	}

	namespace FileDialogs
	{
		std::filesystem::path OpenFileDialog(const std::string& title = "", const std::string& defaultpath = "", const char* filter = nullptr, const char* fileDescription = nullptr);

		std::filesystem::path SaveFileDialog(const std::string& title = "", const std::string& defaultpath = "", const char* filter = nullptr, const char* fileDescription = nullptr);
	}

	template <typename T>
	class StateMachine
	{
	public:
		static void InitState(const T& state) { m_CurrentState = state; }

		static void GoTo(const T& nextState) { m_CurrentState = nextState; }

		static const T GetState() { return m_CurrentState; }
	private:
		static inline T m_CurrentState;
	};
}