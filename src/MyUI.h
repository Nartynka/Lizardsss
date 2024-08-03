#pragma once

#include <SDL.h>

namespace MyUI
{	
	void Init(SDL_Window* window, SDL_Renderer* renderer);
	void ProcessEvent(SDL_Event event);
	void StartFrame();

	void DrawMenu();

	void DrawColorPicker();
	void EndFrame(SDL_Renderer* renderer);
	void Shutdown();
}