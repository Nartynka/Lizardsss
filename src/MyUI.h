#pragma once
#include <SDL.h>
#include <functional>

#include "WormOptions.h"

namespace MyUI
{
	void Init(SDL_Window* window, SDL_Renderer* renderer);
	void ProcessEvent(SDL_Event event);
	void StartFrame();

	void DrawMenu(WormOptions::Options* outOptions, std::function<void()> callback);

	void EndFrame(SDL_Renderer* renderer);
	void Shutdown();

}