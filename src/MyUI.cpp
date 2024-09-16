#include "MyUI.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

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

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 6.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 8.f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(6.f, 4.f));

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
}

void MyUI::DrawMenu(WormOptions::Options* outOptions, std::function<void()> callback)
{
	static ImVec4 outlineColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static ImVec4 faceColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static ImVec4 legsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	static bool eyes = false;
	static bool face = false;
	static bool legs = false;

	ImGui::Begin("Create a Worm!", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SeparatorText("Appearance");

	ImGui::Text("Outline Color:");
	ImGui::SameLine();
	ImGui::ColorEdit3("Outline Color", (float*)&outlineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
	
	ImGui::Checkbox("Eyes", &eyes);
	const char* eyes_types[] = { "Default" };
	static int eyes_current_idx = -1; // Here we store our selection data as an index.
	if (eyes)
	{
		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = eyes_current_idx > -1 ? eyes_types[eyes_current_idx] : "Choose eyes";

		if (ImGui::BeginCombo("##1", combo_preview_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(eyes_types); n++)
			{
				const bool is_selected = (eyes_current_idx == n);
				if (ImGui::Selectable(eyes_types[n], is_selected))
					eyes_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Checkbox("Face", &face);
	const char* face_types[] = { "Smile :)", "Wide Smile :D", "No Smile :("};
	static int face_current_idx = -1; // Here we store our selection data as an index.
	if (face)
	{
		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = face_current_idx > -1 ?  face_types[face_current_idx] : "Choose face";

		if (ImGui::BeginCombo("##2", combo_preview_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(face_types); n++)
			{
				const bool is_selected = (face_current_idx == n);
				if (ImGui::Selectable(face_types[n], is_selected))
					face_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Face Color:");
		ImGui::SameLine();
		ImGui::ColorEdit3("Face Color", (float*)&faceColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);

	}

	ImGui::Checkbox("Legs", &legs);
	if (legs)
	{
		ImGui::Text("Legs Color:");
		ImGui::SameLine();
		ImGui::ColorEdit3("Legs Color", (float*)&legsColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
	}




	ImGui::SeparatorText("Size");
	static bool autoDistance = outOptions->hasAutoDistance;
	static float distance = outOptions->distance;
	static float radius = outOptions->radius;
	static unsigned int numParticles = outOptions->numParticles;

	static float f_min = 1.f;
	static float f_max = 100.f;
	static int i_min = 2;
	static int i_max = 1024;
	ImGui::PushItemWidth(60.f);

	int dragFlags = ImGuiSliderFlags_AlwaysClamp;
	ImGui::DragScalar("Number of particles", ImGuiDataType_U32, &numParticles, 0.02f, &i_min, &i_max, "%u", dragFlags);
	ImGui::DragScalar("Radius of particles", ImGuiDataType_Float, &radius, 1.f, &f_min, &f_max, "%.2f", dragFlags);
	ImGui::Checkbox("Should auto calculate distance?", &autoDistance);
	if (!autoDistance)
	{
		ImGui::DragScalar("Distance between particles", ImGuiDataType_Float, &distance, 1.f, &f_min, &f_max, "%.2f", dragFlags);
	}



	outOptions->hasEyes = eyes;
	outOptions->hasFace = face;
	outOptions->hasLegs = legs;
	outOptions->outlineColor = { (Uint8)(outlineColor.x * 255.0f), (Uint8)(outlineColor.y * 255.0f), (Uint8)(outlineColor.z * 255.0f), 255 };
	outOptions->faceColor = { (Uint8)(faceColor.x * 255.0f), (Uint8)(faceColor.y * 255.0f), (Uint8)(faceColor.z * 255.0f), 255 };
	outOptions->legsColor = { (Uint8)(legsColor.x * 255.0f), (Uint8)(legsColor.y * 255.0f), (Uint8)(legsColor.z * 255.0f), 255 };
	if (face && face_current_idx > -1)
		outOptions->faceType = outOptions->MatchFaceStringToEnum(face_types[face_current_idx]);
	if (eyes && eyes_current_idx > -1)
		outOptions->eyesType = outOptions->MatchEyesStringToEnum(eyes_types[eyes_current_idx]);

	outOptions->hasAutoDistance = autoDistance;
	if (autoDistance)
	{
		outOptions->distance = distance;
	}
	outOptions->distance = distance;
	outOptions->numParticles = numParticles;
	outOptions->radius = radius;


	ImGui::SeparatorText("Magic xD");
	if (ImGui::Button("Create!", { -FLT_MIN, 30.0f }))
	{
		callback();
	}

	ImGui::End();

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

