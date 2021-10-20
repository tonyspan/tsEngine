#include "HierarchyPanel.h"

// NOTE: No dynamic loading of scripts :(
#include "../Scripts/BasicController.h"
#include "../Scripts/DefaultScript.h"
#include "../Scripts/BasicPhysics.h"

HierarchyPanel::HierarchyPanel(const tsEngine::Ref<tsEngine::EntityManager>& context, const tsEngine::Ref<tsEngine::RenderManager>& context2)
{
	SetContext(context, context2);
}

void HierarchyPanel::SetContext(const tsEngine::Ref<tsEngine::EntityManager>& context, const tsEngine::Ref<tsEngine::RenderManager>& context2)
{
	m_Context = context;
	m_Context2 = context2;
	m_SelectedEntity = entt::null;
}

void HierarchyPanel::OnImGuiRender()
{
	bool active = false;
	const ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove /*| ImGuiWindowFlags_NoBackground*/ | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Scene Hierarchy", &active, winFlags);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	static float w = 400.0f;
	static float h = 400.0f;
	ImGui::BeginChild("child1", ImVec2(w, h), true);
	ImGui::LabelText("", "Workspace");
	ImGui::Separator();

	EntityHierarchy();

	//ImGui::Separator();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, h));
	if (ImGui::IsItemActive())
		w += ImGui::GetIO().MouseDelta.x;

	ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
	if (ImGui::IsItemActive())
		h += ImGui::GetIO().MouseDelta.y;

	ImGui::BeginChild("child3", ImVec2(w, 0), true);
	ImGui::LabelText("", "Properties");
	ImGui::Separator();

	Properties();

	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::InvisibleButton("vsplitter2", ImVec2(8.0f, h));
	if (ImGui::IsItemActive())
		w += ImGui::GetIO().MouseDelta.x;

	ImGui::PopStyleVar();

	ImGui::End();
}

void HierarchyPanel::EntityHierarchy()
{
	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	if (ImGui::Button("+", ImVec2(contentRegionAvailable.x, 0.0f)))
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

	m_Context->m_Registry.each([&](auto entity)
	{
		DrawEntityNode(entity);
	});

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		m_SelectedEntity = entt::null;

	contentRegionAvailable = ImGui::GetContentRegionAvail();
	ImGui::InvisibleButton("##LoadWorkspace", ImVec2(contentRegionAvailable.x, contentRegionAvailable.y));
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::filesystem::path loadPath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

			if (loadPath.extension().string() != ".tse")
			{
				LOG_WARN("Could not load {} - not a workspace file", loadPath.filename().string());
			}
			else
			{
				m_FileToBeDeserialized = loadPath;
			}
		}
		ImGui::EndDragDropTarget();
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
		if (ImGui::MenuItem("Delete Entity") /*&& m_Context->GetComponent<tsEngine::TagComponent>(entity).Tag != "MainCamera"*/)
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
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
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
		if (ImGui::MenuItem("Camera Component"))
		{
			if (!m_Context->HasComponent<tsEngine::CameraComponent>(entity))
				m_Context->AddComponent<tsEngine::CameraComponent>(entity);
			else
				LOG_WARN("This entity already has the Camera Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Transform Component"))
		{
			if (!m_Context->HasComponent<tsEngine::TransformComponent>(entity))
				m_Context->AddComponent<tsEngine::TransformComponent>(entity);
			else
				LOG_WARN("This entity already has the Transform Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Sprite Component"))
		{
			if (!m_Context->HasComponent<tsEngine::SpriteComponent>(entity))
				m_Context->AddComponent<tsEngine::SpriteComponent>(entity);
			else
				LOG_WARN("This entity already has the Sprite Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Circle Component"))
		{
			if (!m_Context->HasComponent<tsEngine::CircleComponent>(entity))
				m_Context->AddComponent<tsEngine::CircleComponent>(entity);
			else
				LOG_WARN("This entity already has the Circle Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Mover Component"))
		{
			if (!m_Context->HasComponent<tsEngine::MoverComponent>(entity))
				m_Context->AddComponent<tsEngine::MoverComponent>(entity);
			else
				LOG_WARN("This entity already has the Mover Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Collision Component"))
		{
			if (!m_Context->HasComponent<tsEngine::CollisionComponent>(entity))
			{
				m_Context->AddComponent<tsEngine::CollisionComponent>(entity);
				// NOTE: By default CollisionShape is Circle
				m_Context->GetComponent<tsEngine::CollisionComponent>(entity).Shape = tsEngine::CollisionShape::AABox;
			}
			else
				LOG_WARN("This entity already has the Collision Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Text Component"))
		{
			if (!m_Context->HasComponent<tsEngine::TextComponent>(entity))
				m_Context->AddComponent<tsEngine::TextComponent>(entity);
			else
				LOG_WARN("This entity already has the Text Component");
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("NativeScript Component"))
		{
			if (!m_Context->HasComponent<tsEngine::NativeScriptComponent>(entity))
				m_Context->AddNativeScript<tsEngine::NativeScriptComponent>(entity).Bind<DefaultScript>();
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

		ImGui::Button("Place Texture Here", ImVec2(235.0f, 0.0f));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

				tsEngine::AssetManager::CreateTexture(m_Context2->GetRenderer(), texturePath.string());
				component.Image = tsEngine::AssetManager::GetTexture(texturePath.string());
			}
			ImGui::EndDragDropTarget();
		}
		
		ImGui::SameLine();
		std::string msg = "Textures located on: " + tsEngine::AssetManager::s_AssetPath.string();
		tsEngine::ImGuiHelpMarker(msg.c_str());

		ImGui::Columns(1);

		if (ImGui::BeginTable("##props", 2))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("FlipHorizontal", &component.FlipHorizontal);
			ImGui::TableNextColumn(); ImGui::Checkbox("FlipVertical", &component.FlipVertical);
			ImGui::TableNextColumn(); ImGui::Checkbox("DrawPhysicsCollider", &component.DrawPhysicsCollider);
			ImGui::TableNextColumn(); ImGui::Checkbox("HasAnimation", &component.Animation);
			ImGui::EndTable();
			
			if (component.Animation)
			{
				ImGui::DragInt("Total Frames", &component.AnimationFrames, 1.0f, 1, 60);
				ImGui::DragInt("Current Frame", &component.AnimationCurrentFrame, 1.0f, 0, 60);
				ImGui::DragInt("Height", &component.Height, 1.0f, 1, 200);
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
		if (ImGui::InputText("Text", buffer, sizeof(buffer)))
		{
			component.Text = std::string(buffer);
		}

		glm::vec4 colorf = tsEngine::Maths::U8VecToFloatVec(component.Color);
		ImGui::ColorEdit4("Color", glm::value_ptr(colorf));
		component.Color = tsEngine::Maths::FloatVecToU8Vec(colorf);

		static const int multiplier = component.MultiplierFactor;
		ImGui::DragInt("Multiplier", &component.MultiplierFactor, 0.5f, multiplier, 20);

		ImGui::Button("Place Font Here"/*, ImVec2(235.0f, 0.0f)*/);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path fontPath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

				tsEngine::AssetManager::AddFont(fontPath.string(), 10);
				tsEngine::AssetManager::LoadFont(m_Context2->GetRenderer(), fontPath.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		std::string msg = "Fonts located on: " + tsEngine::AssetManager::s_AssetPath.string();
		tsEngine::ImGuiHelpMarker(msg.c_str());

	});

	// TODO: Dynamic script loading
	DrawComponent<tsEngine::NativeScriptComponent>("Native Script Component", m_Context, entity, [&](auto& component)
	{
		ImGui::Text("Scripts");
		std::string msg = "Script(s) located on: " + tsEngine::AssetManager::s_SciptPath.string();
		ImGui::SameLine();
		tsEngine::ImGuiHelpMarker(msg.c_str());
		ImGui::Separator();

		static int e = 0;
		ImGui::RadioButton("Default", &e, 0);
		ImGui::RadioButton("BasicController", &e, 1);
		ImGui::RadioButton("Physics", &e, 2);

		switch (e)
		{
		case 0:
		{
			tsEngine::Application::Get().UnloadScript(entity);
			tsEngine::Application::Get().LoadScript<DefaultScript>(entity);
			break;
		}
		case 1:
		{
			tsEngine::Application::Get().UnloadScript(entity);
			tsEngine::Application::Get().LoadScript<BasicController>(entity);
			break;
		}
		case 2:
		{
			tsEngine::Application::Get().UnloadScript(entity);
			tsEngine::Application::Get().LoadScript<BasicPhysics>(entity);
			break;
		}
		default:
			break;
		}
	});
}
