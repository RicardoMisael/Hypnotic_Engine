#include "UserInterface.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
void 
UserInterface::init(void* window, 
                    ID3D11Device* device, 
                    ID3D11DeviceContext* deviceContext) {
  IMGUI_CHECKVERSION(); // Check ImGUI version
  ImGui::CreateContext(); // Initialize the context
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup GUI Style
  NataStyle();

  // / When viewports are enabled we tweak WindowRounding/WindoBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform and Renderer bindings
  ImGui_ImplWin32_Init(window);
  ImGui_ImplDX11_Init(device, deviceContext);

}

void 
UserInterface::update() {
  // Start the Dear ImGui frame
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void 
UserInterface::render() {
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  ImGuiIO& io = ImGui::GetIO();

  // Update and Render additional Platform Windows
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(); // Update default platform for Windows
    //if (ImGui::DragFloat3("Posición", reinterpret_cast<float*>(&g_app.position), 0.1f)) {
  }
}

void 
UserInterface::destroy() {
  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

void
UserInterface::NataStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // Fondo oscuro con acentos neón
  colors[ImGuiCol_Text] = ImVec4(0.90f, 1.00f, 0.90f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

  colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 0.50f, 0.50f); // Verde neón
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

  colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);

  colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 1.00f, 0.60f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);

  colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.20f, 1.00f, 1.00f); // Morado eléctrico

  colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);

  colors[ImGuiCol_Button] = ImVec4(0.00f, 1.00f, 0.50f, 0.80f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.70f, 0.30f, 1.00f);

  colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 0.50f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.70f, 0.30f, 1.00f);

  colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);

  colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.70f, 0.30f, 1.00f);

  colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.85f, 0.40f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.15f, 0.20f, 1.00f);

  colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.07f, 0.10f, 0.15f, 1.00f);

  // Ajustes generales de estilo
  style.WindowRounding = 5.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.FrameBorderSize = 1.0f;
  style.WindowBorderSize = 1.0f;
}

void
UserInterface::hierarchy(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Hierarchy");

  for (int i = 0; i < actors.size(); ++i) {
    auto& actor = actors[i];
    if (actor.isNull()) continue;

    ImGui::PushID(i);
    std::string displayName = std::to_string(i) + " - " + actor->getName();
    if (ImGui::Selectable(displayName.c_str(), selectedActor == actor)) {
      selectedActor = actor;
    }
    ImGui::PopID();
  }

  ImGui::End();
}

void 
UserInterface::inspector(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Inspector");

  for (int i = 0; i < actors.size(); ++i) {
    auto& actor = actors[i];
    if (actor.isNull()) continue;

    ImGui::PushID(i); // Necesario para que ImGui distinga cada bloque

    // Muestra el nombre del actor
    ImGui::Text("%s", actor->getName().c_str());

    // Obtener el transform
    auto transform = actor->getComponent<Transform>();
    if (!transform.isNull()) {
      // Posición
      EngineUtilities::Vector3 position = transform->getPosition();
      float pos[3] = { position.x, position.y, position.z };
      if (ImGui::DragFloat3("Position", pos, 0.1f)) {
        transform->setPosition(EngineUtilities::Vector3(pos[0], pos[1], pos[2]));
      }

      // Rotación
      EngineUtilities::Vector3 rotation = transform->getRotation();
      float rot[3] = { rotation.x, rotation.y, rotation.z };
      if (ImGui::DragFloat3("Rotation", rot, 0.1f)) {
        transform->setRotation(EngineUtilities::Vector3(rot[0], rot[1], rot[2]));
      }

      // Escala
      EngineUtilities::Vector3 scale = transform->getScale();
      float scl[3] = { scale.x, scale.y, scale.z };
      if (ImGui::DragFloat3("Scale", scl, 0.1f)) {
        transform->setScale(EngineUtilities::Vector3(scl[0], scl[1], scl[2]));
      }
    }

    ImGui::Separator();
    ImGui::PopID();
  }

  ImGui::End();
}