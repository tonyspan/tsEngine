#pragma once

#include "Engine/Core/Base.h"

#include "Engine/ECS/EntityManager.h"

#include <yaml-cpp/yaml.h>

namespace tsEngine
{

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