#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"

#include "entt.hpp"

#include <glm/glm.hpp>

namespace tsEngine
{
	class Texture;
	class Font;

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec2 Position{};
		glm::vec2 Size{};
		float Rotation = 0.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::vec2 pos)
			: Position(pos) {}
		TransformComponent(glm::vec2 pos, glm::vec2 size)
			: Position(pos), Size(size) {}
		TransformComponent(float posx, float posy)
			: Position(posx, posy) {}
		TransformComponent(float posx, float posy, float sizex, float sizey)
			: Position(posx, posy), Size(sizex, sizey) {}
	};

	struct MoverComponent
	{
		glm::vec2 TranslationSpeed{};
		float RotationSpeed = 0.0f;

		MoverComponent() = default;
		MoverComponent(float x, float y)
			: TranslationSpeed(x, y) {}
		MoverComponent(float x, float y, float r)
			: TranslationSpeed(x, y), RotationSpeed(r) {}
	};

	struct SpriteComponent
	{
		// u8vec4 compatible with SDL_Color
		glm::u8vec4 Color = { 255, 255, 255, 0 };

		Ref<Texture> Image = nullptr;
		bool FlipHorizontal = false;
		bool FlipVertical = false;

		bool DrawPhysicsCollider = false;

		bool HasAnimation = false;
		int AnimationFrames = 1;
		int AnimationCurrentFrame = 0;
		int Width = 1;
		int Height = 1;
		int DelayPerFrame = 150;
	};

	struct CircleComponent
	{
		// u8vec4 compatible with SDL_Color
		glm::u8vec4 Color = { 255, 255, 255, 0 };

		bool DrawPhysicsCollider = false;
	};

	struct CameraComponent
	{
		float PanSpeed;

		CameraComponent() = default;
		CameraComponent(float speed)
			: PanSpeed(speed) {}
	};

	enum class CollisionShape
	{
		AABox = 0,
		Circle,
	};

	struct CollisionComponent
	{
		glm::vec2 Size{};
		CollisionShape Shape = CollisionShape::Circle;
		std::set<entt::entity> CollidedWith;

		CollisionComponent() = default;
		CollisionComponent(float radius)
			: Size(radius) {}
		CollisionComponent(float x, float y, CollisionShape shape = CollisionShape::AABox)
			: Size(x, y), Shape(shape) {}
	};

	struct TextComponent
	{
		Ref<Font> Font = nullptr;
		std::string Text = std::string();
		int MultiplierFactor = 4;
		int WrapWidth = 0;
		bool HasBorder = false;

		// u8vec4 compatible with SDL_Color
		glm::u8vec4 Color = { 255, 255, 255, 0 };

		//TextComponent(const std::string& text = "Enter Text")
		//    : Text(text) {}
	};

	struct AudioComponent
	{
		std::string MusicPath;

		std::string SoundPath;
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		Ref<ScriptableEntity> Instance = nullptr;

		Ref<ScriptableEntity>(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return std::dynamic_pointer_cast<ScriptableEntity>(CreateRef<T>()); };
			DestroyScript = [](NativeScriptComponent* nsc) { nsc->Instance = nullptr; };
		}
	};
}