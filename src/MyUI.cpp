#include "MyUI.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "WormOptions.h"

void MyUI::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
}

void MyUI::ProcessEvent(SDL_Event event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);  // Forward event to imgui backend
}

void MyUI::StartFrame()
{
	// (After event loop)
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	//ImGui::ShowDemoWindow(); // Show demo window! :)
	DrawMenu();
}

void MyUI::DrawMenu()
{
	ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
	ImGui::Begin("Create a Worm!");
	ImGui::SeparatorText("Options");

	static ImVec4 outlineColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
	ImGui::Text("Outline Color:");
	ImGui::SameLine();
	ImGui::ColorEdit3("Outline Color", (float*)&outlineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);

	static bool eyes = false;
	static bool face = false;
	ImGui::Checkbox("Eyes", &eyes);

	if (eyes)
	{
		const char* items[] = { "Default" };
		static int item_current_idx = -1; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = item_current_idx > -1 ? items[item_current_idx] : "Choose eyes";

		if (ImGui::BeginCombo("##1", combo_preview_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Checkbox("Face", &face);
	if (face)
	{
		const char* items[] = { "Smile :)", "Wide Smile :D", "No Smile :("};
		static int item_current_idx = -1; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = item_current_idx > -1 ?  items[item_current_idx] : "Choose face";

		if (ImGui::BeginCombo("##2", combo_preview_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		static ImVec4 faceColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
		ImGui::Text("Face Color:");
		ImGui::SameLine();
		ImGui::ColorEdit3("Face Color", (float*)&faceColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);

	}

	ImGui::SeparatorText("Magic xD");
	ImGui::Button("Create!", sz);
	ImGui::End();
}


void MyUI::DrawColorPicker()
{
}


void MyUI::EndFrame(SDL_Renderer* renderer)
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void MyUI::Shutdown()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

