#include "HierarchyPanel.h"

#include "../Scripts/BasicController.h"
#include "../Scripts/DefaultScript.h"
#include "../Scripts/BasicPhysics.h"

#include "imgui.h"

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

HierarchyPanel::HierarchyPanel(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
{
	SetContext(entityManager);
}

void HierarchyPanel::SetContext(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
{
	m_Context = entityManager;
	m_SelectedEntity = entt::null;
}

void HierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	EntityHierarchy();

	ImGui::End();

	ImGui::Begin("Properties");

	Properties();

	ImGui::End();
}

void HierarchyPanel::EntityHierarchy()
{
	m_Context->m_Registry.each([&](auto entity)
	{
		DrawEntityNode(entity);
	});

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		m_SelectedEntity = entt::null;
	
	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	if (ImGui::InvisibleButton("##AddEntity", contentRegionAvailable, ImGuiButtonFlags_MouseButtonRight))
	{
		ImGui::OpenPopup("AddEntity");
	}
	if (ImGui::BeginPopup("AddEntity"))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			auto entity = m_Context->CreateEntity("Empty Entity");
			m_Context->AddComponent<tsEngine::TransformComponent>(entity, 0.0f, 0.0f, 100.0f, 100.0f);
		}
		ImGui::EndPopup();
	}
}

void HierarchyPanel::Properties()
{
	if (m_SelectedEntity != entt::null)
	{
		DrawComponents(m_SelectedEntity);
	}
}

void HierarchyPanel::SetSelectedEntity(entt::entity entity)
{
	m_SelectedEntity = entity;
}

void HierarchyPanel::DrawEntityNode(entt::entity entity)
{
	auto& tag = m_Context->GetComponent<tsEngine::TagComponent>(entity).Tag;

	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
		ImGui::TreePop();

	if (entityDeleted)
	{
		m_Context->DestroyEntity(entity);
		if (m_SelectedEntity == entity)
			m_SelectedEntity = entt::null;
	}
}

static void DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = 10.0f * 2.0f;
	float speed = 1.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");

	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawFloatControl(const std::string& label, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = 10.0f * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	if (ImGui::Button("R", buttonSize))
		*value = resetValue;

	ImGui::SameLine();
	ImGui::DragFloat("##X", value, speed, min, max, "%.2f");

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename FUNC>
static void DrawComponent(const std::string& name, const tsEngine::Ref<tsEngine::EntityManager>& entityManager, entt::entity entity, FUNC uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
											ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entityManager->HasComponent<T>(entity))
	{
		auto& component = entityManager->GetComponent<T>(entity);
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		float lineHeight = 10.0f * 2.0f;
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entityManager->RemoveComponent<T>(entity);
	}
}

void HierarchyPanel::DrawComponents(entt::entity entity)
{
	if (m_Context->HasComponent<tsEngine::TagComponent>(entity))
	{
		auto& tag = m_Context->GetComponent<tsEngine::TagComponent>(entity).Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DrawComponentItem<tsEngine::CameraComponent>("Camera Component", entity);
		DrawComponentItem<tsEngine::TransformComponent>("Transform Component", entity);
		DrawComponentItem<tsEngine::SpriteComponent>("Sprite Component", entity);
		DrawComponentItem<tsEngine::CircleComponent>("Circle Component", entity);
		DrawComponentItem<tsEngine::MoverComponent>("Mover Component", entity);

		DrawComponentItem<tsEngine::CollisionComponent>("Collision Component", entity, [](auto& entityManager, auto entity) {
			// NOTE: By default CollisionShape is Circle
			entityManager->GetComponent<tsEngine::CollisionComponent>(entity).Shape = tsEngine::CollisionShape::AABox;
		});

		DrawComponentItem<tsEngine::TextComponent>("Text Component", entity, [](auto& entityManager, auto entity) {
			auto& t = entityManager->GetComponent<tsEngine::TextComponent>(entity);
			t.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("default");
			t.Text = "Enter Text";
		});

		DrawComponentItem<tsEngine::AudioComponent>("Audio Component", entity);

		if (ImGui::MenuItem("NativeScript Component"))
		{
			if (!m_Context->HasComponent<tsEngine::NativeScriptComponent>(entity))
				m_Context->AddComponent<tsEngine::NativeScriptComponent>(entity).Bind<DefaultScript>();
			else
				LOG_WARN("This entity already has the NativeScript Component");
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	DrawComponent<tsEngine::CameraComponent>("Camera Component", m_Context, entity, [](auto& component)
	{
		DrawFloatControl("Pan Speed", &component.PanSpeed, 1.0f, 0.0f, 200.0f);
	});

	// NOTE: Default resetValue for Size is 100.0f (forced)
	DrawComponent<tsEngine::TransformComponent>("Transform Component", m_Context, entity, [](auto& component)
	{
		DrawVec2Control("Position", component.Position);
		DrawVec2Control("Size", component.Size, 100.0f);
		DrawFloatControl("Rotation", &component.Rotation, 1.0f, 0.0f, 360.0f);
	});

	DrawComponent<tsEngine::SpriteComponent>("Sprite Component", m_Context, entity, [&](auto& component)
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Path");
		ImGui::NextColumn();

		/*static*/ glm::vec4 colorf = tsEngine::Maths::U8VecToFloatVec(component.Color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorf));
		component.Color = tsEngine::Maths::FloatVecToU8Vec(colorf);
		
		ImGui::SameLine();
		std::string msg = "Textures located on: " + tsEngine::AssetManager::s_AssetPath.string();
		msg += "\nSelect an entity (hierarchy panel or viewwport) and drag a texture to viewport";
		ImGuiHelpMarker(msg.c_str());

		ImGui::Columns(1);

		if (ImGui::BeginTable("##props", 2))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("FlipHorizontal", &component.FlipHorizontal);
			ImGui::TableNextColumn(); ImGui::Checkbox("FlipVertical", &component.FlipVertical);
			ImGui::TableNextColumn(); ImGui::Checkbox("DrawPhysicsCollider", &component.DrawPhysicsCollider);
			ImGui::TableNextColumn(); ImGui::Checkbox("HasAnimation", &component.HasAnimation);
			ImGui::EndTable();
			
			if (component.HasAnimation)
			{
				ImGui::DragInt("Total Frames", &component.AnimationFrames, 1.0f, 1, 60);
				ImGui::DragInt("Current Frame", &component.AnimationCurrentFrame, 1.0f, 0, 60);
				ImGui::DragInt("Width", &component.Width, 1.0f, 1, 1024);
				ImGui::DragInt("Height", &component.Height, 1.0f, 1, 1024);
				ImGui::DragInt("Delay", &component.DelayPerFrame, 1.0f, 50, 300);
			}
		}
	});

	DrawComponent<tsEngine::CircleComponent>("Circle Component", m_Context, entity, [&](auto& component)
	{
		/*static*/ glm::vec4 colorf = tsEngine::Maths::U8VecToFloatVec(component.Color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorf));
		component.Color = tsEngine::Maths::FloatVecToU8Vec(colorf);

		ImGui::Checkbox("DrawPhysicsCollider", &component.DrawPhysicsCollider);
	});

	DrawComponent<tsEngine::MoverComponent>("Mover Component", m_Context, entity, [](auto& component)
	{
		DrawVec2Control("Transl. Speed", component.TranslationSpeed);
		DrawFloatControl("Rot. Speed", &component.RotationSpeed, 1.0f, 0.0f, 200.0f);
	});

	DrawComponent<tsEngine::CollisionComponent>("Collision Component", m_Context, entity, [&](auto& component)
	{
		static bool followEntitySize = true;
			
		DrawVec2Control("Size", component.Size);
		ImGui::Checkbox("Same as Entity", &followEntitySize);

		if (followEntitySize)
			component.Size = m_Context->GetComponent<tsEngine::TransformComponent>(entity).Size;

		if (m_Context->HasComponent<tsEngine::SpriteComponent>(entity))
			component.Shape = tsEngine::CollisionShape::AABox;
	});

	DrawComponent<tsEngine::TextComponent>("Text Component", m_Context, entity, [&](auto& component)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, component.Text.c_str(), sizeof(buffer));
		if (ImGui::InputTextMultiline("Text", buffer, sizeof(buffer)))
		{
			component.Text = std::string(buffer);
		}

		glm::vec4 colorf = tsEngine::Maths::U8VecToFloatVec(component.Color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorf));
		component.Color = tsEngine::Maths::FloatVecToU8Vec(colorf);

		static const int multiplier = component.MultiplierFactor;
		static const int multiplier2 = component.WrapWidth;
		ImGui::DragInt("Multiplier", &component.MultiplierFactor, 0.5f, multiplier, 20);
		ImGui::DragInt("WrapWidth", &component.WrapWidth, 0.5f, multiplier2, 50);
		ImGui::Checkbox("Border", &component.HasBorder);

		ImGui::Button("Place Font Here", ImVec2(235.0f, 0.0f));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path filePath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

				auto asset = tsEngine::Asset::IsSupported(filePath);

				if (asset.Type == tsEngine::AssetType::Font)
				{
					auto font = tsEngine::Asset::Create<tsEngine::Font>(filePath.string(), 10);
					tsEngine::AssetManager::AddAsset<tsEngine::Font>(filePath.string(), font);
					component.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>(filePath.string());
				}
				else
				{
					LOG_WARN("Not a font format");
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		std::string msg = "Fonts located on: " + tsEngine::AssetManager::s_AssetPath.string();
		ImGuiHelpMarker(msg.c_str());

	});

	DrawComponent<tsEngine::AudioComponent>("Audio Component", m_Context, entity, [&](auto& component)
	{
			// TODO
	});

	// TODO: Dynamic script loading
	DrawComponent<tsEngine::NativeScriptComponent>("Native Script Component", m_Context, entity, [&](auto& component)
	{
		ImGui::Text("Scripts");
		std::string msg = "Script(s) located on: " + tsEngine::AssetManager::s_SciptPath.string();
		ImGui::SameLine();
		ImGuiHelpMarker(msg.c_str());
		ImGui::Separator();

		static int e = 0;
		ImGui::RadioButton("Default", &e, 0);
		ImGui::RadioButton("BasicController", &e, 1);
		ImGui::RadioButton("Physics", &e, 2);

		switch (e)
		{
			case 0:
			{
				tsEngine::NativeScript::UnloadScript(entity);
				tsEngine::NativeScript::LoadScript<DefaultScript>(entity);
				break;
			}
			case 1:
			{
				tsEngine::NativeScript::UnloadScript(entity);
				tsEngine::NativeScript::LoadScript<BasicController>(entity);
				break;
			}
			case 2:
			{
				tsEngine::NativeScript::UnloadScript(entity);
				tsEngine::NativeScript::LoadScript<BasicPhysics>(entity);
				break;
			}
			default:
				break;
		}
	});
}
