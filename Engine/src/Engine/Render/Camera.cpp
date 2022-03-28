#include "pch.h"

#include "Camera.h"

namespace tsEngine
{
	CameraData::CameraData(const glm::vec2& position, const glm::vec2 size)
	{
		Set(position, size);
	}

	void CameraData::Set(const glm::vec2& position, const glm::vec2& size)
	{
		m_Position = position;
		m_Size = size;
	}

	const glm::vec2& CameraData::GetPosition() const
	{
		return m_Position;
	}

	const glm::vec2& CameraData::GetSize() const
	{
		return m_Size;
	}
}