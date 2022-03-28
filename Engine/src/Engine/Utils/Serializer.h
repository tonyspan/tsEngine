#pragma once

#include "Engine/Core/Base.h"

#include "entt.hpp"

#include <yaml-cpp/yaml.h>

namespace tsEngine
{
	class EntityManager;

	class Serializer
	{
	public:
		Serializer(const Ref<EntityManager>& entityManager);

		void Serialize(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
	private:
		void SerializeEntity(YAML::Emitter& out, entt::entity entity);
	private:
		Ref<EntityManager> m_Context;
	};

}