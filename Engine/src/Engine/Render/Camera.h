#pragma once

#include <glm/glm.hpp>

namespace tsEngine
{
	class CameraData
	{
	public:
		CameraData() = default;
		CameraData(const glm::vec2& position, const glm::vec2 size);

		~CameraData() = default;

		void Set(const glm::vec2& position, const glm::vec2& size);

		const glm::vec2& GetPosition() const;
		const glm::vec2& GetSize() const;
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
	};
}