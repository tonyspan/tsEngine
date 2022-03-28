#include "pch.h"

#include "Serializer.h"

#include "Engine/Log/Log.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

#include "Engine/Render/Texture.h"
#include "Engine/Render/Font.h"

#include "Engine/Asset/AssetManager.h"
#include "Engine/Utils/Utils.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			
			return true;
		}
	};

}
namespace tsEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	Serializer::Serializer(const Ref<EntityManager>& entityManager)
		: m_Context(entityManager)
	{
	}

	static CollisionShape CollisionShapeFromString(const std::string& collisionShapeString)
	{
		if (collisionShapeString == "AABox") return CollisionShape::AABox;
		if (collisionShapeString == "Circle") return CollisionShape::Circle;

		ASSERT(false, "Unknown collision shape");
		
		return CollisionShape::AABox;
	}

	static std::string CollisionShapeToString(const CollisionShape& collisionShape)
	{
		if (collisionShape == CollisionShape::AABox) return "AABox";
		if (collisionShape == CollisionShape::Circle) return "Circle";

		ASSERT(false, "Unknown collision shape");

		return "AABox";
	}

	void Serializer::SerializeEntity(YAML::Emitter& out, entt::entity entity)
	{
		ASSERT(m_Context->HasComponent<TagComponent>(entity), "Entity does not have TagComponent");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (m_Context->HasComponent<TagComponent>(entity))
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = m_Context->GetComponent<TagComponent>(entity).Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (m_Context->HasComponent<TransformComponent>(entity))
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = m_Context->GetComponent<TransformComponent>(entity);
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Size" << YAML::Value << tc.Size;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;

			out << YAML::EndMap; // TransformComponent
		}

		if (m_Context->HasComponent<MoverComponent>(entity))
		{
			out << YAML::Key << "MoverComponent";
			out << YAML::BeginMap; // MoverComponent

			auto& mc = m_Context->GetComponent<MoverComponent>(entity);
			out << YAML::Key << "TranslationSpeed" << YAML::Value << mc.TranslationSpeed;
			out << YAML::Key << "RotationSpeed" << YAML::Value << mc.RotationSpeed;

			out << YAML::EndMap; // MoverComponent
		}

		if (m_Context->HasComponent<CameraComponent>(entity))
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cc = m_Context->GetComponent<CameraComponent>(entity);

			out << YAML::Key << "PanSpeed" << YAML::Value << cc.PanSpeed;

			out << YAML::EndMap; // CameraComponent
		}

		if (m_Context->HasComponent<SpriteComponent>(entity))
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			auto& sc = m_Context->GetComponent<SpriteComponent>(entity);
			out << YAML::Key << "Color" << YAML::Value << Maths::U8VecToFloatVec(sc.Color);
			
			if (sc.Image)
				out << YAML::Key << "Image" << YAML::Value << std::filesystem::relative(sc.Image->GetPath(), AssetManager::s_BasePath).string();
			else
				out << YAML::Key << "Image" << YAML::Value << YAML::Null;

			out << YAML::Key << "FlipHorizontal" << YAML::Value << sc.FlipHorizontal;
			out << YAML::Key << "FlipVertical" << YAML::Value << sc.FlipVertical;
			out << YAML::Key << "DrawPhysicsCollider" << YAML::Value << sc.DrawPhysicsCollider;

			out << YAML::Key << "Animation" << YAML::Value << sc.HasAnimation;
			out << YAML::Key << "AnimationFrames" << YAML::Value << sc.AnimationFrames;
			out << YAML::Key << "AnimationCurrentFrame" << YAML::Value << sc.AnimationCurrentFrame;
			out << YAML::Key << "Width" << YAML::Value << sc.Width;
			out << YAML::Key << "Height" << YAML::Value << sc.Height;
			out << YAML::Key << "DelayPerFrame" << YAML::Value << sc.DelayPerFrame;

			out << YAML::EndMap; // SpriteComponent
		}

		if (m_Context->HasComponent<CircleComponent>(entity))
		{
			out << YAML::Key << "CircleComponent";
			out << YAML::BeginMap; // CircleComponent

			auto& cc = m_Context->GetComponent<CircleComponent>(entity);
			out << YAML::Key << "Color" << YAML::Value << Maths::U8VecToFloatVec(cc.Color);
			out << YAML::Key << "DrawPhysicsCollider" << YAML::Value << cc.DrawPhysicsCollider;

			out << YAML::EndMap; // CircleComponent
		}

		if (m_Context->HasComponent<CollisionComponent>(entity))
		{
			out << YAML::Key << "CollisionComponent";
			out << YAML::BeginMap; // CollisionComponent

			auto& cc = m_Context->GetComponent<CollisionComponent>(entity);
			out << YAML::Key << "Shape" << YAML::Value << CollisionShapeToString(cc.Shape);
			out << YAML::Key << "Size" << YAML::Value << cc.Size;

			out << YAML::EndMap; // CollisionComponent
		}

		if (m_Context->HasComponent<TextComponent>(entity))
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap; // TextComponent

			auto& tc = m_Context->GetComponent<TextComponent>(entity);
			out << YAML::Key << "Font" << YAML::Value << std::filesystem::relative(tc.Font->GetPath(), AssetManager::s_BasePath).string();
			out << YAML::Key << "FontSize" << YAML::Value << tc.Font->FontSize;
			out << YAML::Key << "Text" << YAML::Value << tc.Text;
			out << YAML::Key << "MultiplierFactor" << YAML::Value << tc.MultiplierFactor;
			out << YAML::Key << "WrapWidth" << YAML::Value << tc.WrapWidth;
			out << YAML::Key << "Border" << YAML::Value << tc.HasBorder;
			out << YAML::Key << "Color" << YAML::Value << Maths::U8VecToFloatVec(tc.Color);

			out << YAML::EndMap; // TextComponent
		}

		out << YAML::EndMap; // Entity
	}

	void Serializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Workspace" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Context->m_Registry.each([&](auto entity)
		{
			if (entity == entt::null)
				return;

			SerializeEntity(out, entity);
		});
		
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool Serializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Workspace"])
			return false;

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();
#if 0
				LOG_INFO("Deserialized entity with name = {}", name);
#endif
				auto deserializedEntity = m_Context->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					m_Context->AddComponent<TransformComponent>(deserializedEntity);
					auto& tc = m_Context->GetComponent<TransformComponent>(deserializedEntity);

					tc.Position = transformComponent["Position"].as<glm::vec2>();
					tc.Size = transformComponent["Size"].as<glm::vec2>();
					tc.Rotation = transformComponent["Rotation"].as<float>();
				}

				auto moverComponent = entity["MoverComponent"];
				if (moverComponent)
				{
					m_Context->AddComponent<MoverComponent>(deserializedEntity);
					auto& mc = m_Context->GetComponent<MoverComponent>(deserializedEntity);

					mc.TranslationSpeed = moverComponent["TranslationSpeed"].as<glm::vec2>();
					mc.RotationSpeed = moverComponent["RotationSpeed"].as<float>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					m_Context->AddComponent<CameraComponent>(deserializedEntity);
					auto& cc = m_Context->GetComponent<CameraComponent>(deserializedEntity);

					cc.PanSpeed = cameraComponent["PanSpeed"].as<float>();
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					m_Context->AddComponent<SpriteComponent>(deserializedEntity);
					auto& src = m_Context->GetComponent<SpriteComponent>(deserializedEntity);

					src.Color = Maths::FloatVecToU8Vec(spriteComponent["Color"].as<glm::vec4>());

					auto texNode = spriteComponent["Image"];

					if (!texNode.IsNull())
					{
						auto texPath = texNode.as<std::string>();
						auto fullPath = AssetManager::s_BasePath / texPath;
						auto tex = Asset::Create<Texture>(fullPath.string());
						AssetManager::AddAsset<Texture>(texPath, tex);

						src.Image = AssetManager::GetAsset<Texture>(texPath);
					}
					
					src.FlipHorizontal = spriteComponent["FlipHorizontal"].as<bool>();
					src.FlipVertical = spriteComponent["FlipVertical"].as<bool>();
					src.DrawPhysicsCollider = spriteComponent["DrawPhysicsCollider"].as<bool>();

					src.HasAnimation = spriteComponent["Animation"].as<bool>();
					src.AnimationFrames = spriteComponent["AnimationFrames"].as<int>();
					src.AnimationCurrentFrame = spriteComponent["AnimationCurrentFrame"].as<int>();
					src.Width = spriteComponent["Width"].as<int>();
					src.Height = spriteComponent["Height"].as<int>();
					src.DelayPerFrame = spriteComponent["DelayPerFrame"].as<int>();
				}

				auto circleComponent = entity["CircleComponent"];
				if (circleComponent)
				{
					m_Context->AddComponent<CircleComponent>(deserializedEntity);
					auto& src = m_Context->GetComponent<CircleComponent>(deserializedEntity);

					src.Color = Maths::FloatVecToU8Vec(circleComponent["Color"].as<glm::vec4>());
					src.DrawPhysicsCollider = circleComponent["DrawPhysicsCollider"].as<bool>();
				}

				auto collisionComponent = entity["CollisionComponent"];
				if (collisionComponent)
				{
					m_Context->AddComponent<CollisionComponent>(deserializedEntity);
					auto& cc = m_Context->GetComponent<CollisionComponent>(deserializedEntity);

					cc.Shape = CollisionShapeFromString(collisionComponent["Shape"].as<std::string>());
					cc.Size = collisionComponent["Size"].as<glm::vec2>();
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					m_Context->AddComponent<TextComponent>(deserializedEntity);
					auto& tc = m_Context->GetComponent<TextComponent>(deserializedEntity);

					auto font = textComponent["Font"].as<std::string>();
					auto fullPath = AssetManager::s_BasePath / font;
					auto fontSize = Asset::Create<Font>(fullPath.string(), textComponent["FontSize"].as<int>());
					AssetManager::AddAsset<Font>(fullPath.string(), fontSize);
					
					tc.Font = AssetManager::GetAsset<Font>(fullPath.string());
					tc.Text = textComponent["Text"].as<std::string>();
					tc.MultiplierFactor = textComponent["MultiplierFactor"].as<int>();
					tc.WrapWidth = textComponent["WrapWidth"].as<int>();
					tc.HasBorder = textComponent["Border"].as<bool>();
					tc.Color = Maths::FloatVecToU8Vec(textComponent["Color"].as<glm::vec4>());
				}
			}
		}

		return true;
	}
}
