#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <ctime> 
#include <vector>

#include "Vec2.h"
#include "Utils.h"
#include "Worm.h"
#include "MyUI.h"
#include <memory>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


int main(int argc, char* args[])
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	assert(result == 0 && "SDL could not initialize!");

	SDL_Window* window = SDL_CreateWindow("Lizardsss", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	assert(window != nullptr && "Window could not be created!");

	SDL_Renderer* renderer =  SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer != nullptr && "Renderer could not be created!");


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	MyUI::Init(window, renderer);

	Worm playerWorm(renderer);

	std::vector<std::shared_ptr<Worm>> worms;

	const float fixedDeltaTime = 1.f / 60.f; // 60 FPS
	float lastTime = 0.f;

	srand(time(0));

	bool quit = false;
	SDL_Event event;


	while (!quit)
	{
		const double globalTime = SDL_GetTicks();
		const float dt = (SDL_GetTicks() - lastTime) / 1000;


		if (dt >= fixedDeltaTime)
		{
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
					quit = true;

				MyUI::ProcessEvent(event);
			}

			MyUI::StartFrame();

			WormOptions::Options options;
			MyUI::DrawMenu(&options, [renderer, &options, &worms]() {auto newWorm = std::make_shared<Worm>(renderer, options); worms.emplace_back(newWorm); });

			{
				playerWorm.outlineColor = options.outlineColor;
				playerWorm.hasEyes = options.hasEyes;
				playerWorm.hasFace = options.hasFace;
				playerWorm.faceColor = options.faceColor;
				playerWorm.faceType = options.faceType;
				playerWorm.eyesType = options.eyesType;
			}

			int mouseX = 500;
			int mouseY = 100;
			SDL_GetMouseState(&mouseX, &mouseY);
			Vec2 mousePos = { mouseX, mouseY };

			playerWorm.particles[0].pos = mousePos;

			playerWorm.ResolveConstrains();
			playerWorm.DrawBody(renderer);

			for (const auto& w : worms)
			{
				w->ResolveConstrains();
				w->DrawBody(renderer);
				//w->MoveRandom(fixedDeltaTime);
			}


			MyUI::EndFrame(renderer);

			SDL_SetRenderDrawColor(renderer, 55, 55, 100, 255);
			SDL_RenderPresent(renderer);
			SDL_RenderClear(renderer);

			lastTime = (float)SDL_GetTicks();

		}
	}

	MyUI::Shutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
