#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <ctime> 
#include <vector>

#include "Vec2.h"
#include "Utils.h"
#include "Worm.h"
#include "MyUI.h"


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

	Worm worm(renderer);

	const float fixedDeltaTime = 1.f / 60.f; // 60 FPS
	float lastTime = 0.f;

	srand(time(0));

	bool quit = false;
	SDL_Event event;

	std::vector<Point> points;
	const int numPoints = 25;
	const float dist = 25.f;


	for (int i = 0; i < numPoints; ++i)
	{
		points.push_back(CreatePoint(renderer, { 10*i, 100 }));
	}


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

			int mouseX;
			int mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			Vec2 mousePos = { mouseX, mouseY };

			//points[0].pos = {mousePos.x - points[0].size.x/2, mousePos.y - points[0].size.y / 2 };

			//for (int i = 1; i < numPoints; ++i)
			//{
			//	Point& p1 = points[i];
			//	Point p2 = points[i - 1];
			//	p1.pos = ConstrainDistance(p1.pos, p2.pos, dist);

			//	float of_x = p1.size.x / 2;
			//	float of_y = p1.size.y / 2;

			//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//	SDL_RenderDrawLine(renderer, p1.pos.x + of_x, p1.pos.y + of_y, p2.pos.x + of_x, p2.pos.y + of_y);
			//}

			worm.particles[0].pos = mousePos;
			// resolve constrains
			worm.ResolveConstrains();

			/// DRAW EVERYTHING
			worm.DrawBody(renderer);

			// draw points starting from the last point so that first points are drawn on the last points
			//for (int i = numPoints-1; i >= 0; --i)
			//{
			//	Point p = points[i];
			//	DrawPoint(renderer, p);
			//}

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
