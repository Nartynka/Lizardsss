#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <ctime> 
#include <vector>
#include <memory>

#include "Vec2.h"
#include "Utils.h"
#include "Worm.h"
#include "MyUI.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#define WORM_DEBUG false
// if should draw actual body if in debug
#define DRAW_BODY_DEBUG false

int main(int argc, char* args[])
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	assert(result == 0 && "SDL could not initialize!");

	SDL_Window* window = SDL_CreateWindow("Lizardsss", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	assert(window != nullptr && "Window could not be created!");

	SDL_Renderer* renderer =  SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer != nullptr && "Renderer could not be created!");


	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	MyUI::Init(window, renderer);

	Worm playerWorm(renderer);

	std::vector<std::shared_ptr<Worm>> worms;

	const float fixedDeltaTime = 1.f / 60.f; // 60 FPS
	float lastTime = 0.f;

	srand(time(0));

	bool quit = false;
	SDL_Event event;

	//float vel;
	//float acceleration = 15.f;

	//Leg arm;
	//arm.foot = { 400, 300 };
	//arm.knee = { arm.foot.x, arm.foot.y + arm.thighLenght };
	//arm.hip = { arm.knee.x, arm.knee.y + arm.shinLenght };

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

			//MyUI::DrawMenu(&options, [renderer, &options, &worms]() {auto newWorm = std::make_shared<Worm>(renderer, options); worms.emplace_back(newWorm); });

			// only for preview
			//{
			//	playerWorm.outlineColor = options.outlineColor;
			//	playerWorm.hasEyes = options.hasEyes;
			//	playerWorm.hasFace = options.hasFace;
			//	playerWorm.faceColor = options.faceColor;
			//	playerWorm.faceType = options.faceType;
			//	playerWorm.eyesType = options.eyesType;
			//}

			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			Vec2 mousePos = { mouseX, mouseY };

			playerWorm.particles[0].pos = mousePos;

			//arm.anchor = mousePos;
			//arm.FABRIK();
			//arm.CalculateNextStep(renderer);
			//arm.Draw(renderer);

			playerWorm.ResolveConstrains();
#if !WORM_DEBUG || DRAW_BODY_DEBUG
			playerWorm.DrawBody(renderer);
#endif

#if WORM_DEBUG
			playerWorm.DrawDebugBody(renderer);
#endif
			for (const auto& w : worms)
			{
				w->ResolveConstrains();
				w->DrawBody(renderer);
				w->MoveToRandom(fixedDeltaTime);
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



//struct Leg
//{
//	float thighLenght = 80.f;
//	float shinLenght = 60.f;
//
//	Vec2 foot = { 400, 300 };
//	Vec2 knee = { foot.x, foot.y + thighLenght };
//	Vec2 hip = { knee.x, knee.y + shinLenght }; // attached to body
//
//	Vec2 target = { 500, 200 };
//	Vec2 nextStep = foot;
//
//	Leg()
//	{
//
//	}
//
//	void FABRIK()
//	{
//		// forward reaching
//		hip = target;
//		knee = hip + (knee - hip).normalize() * shinLenght;
//
//		// backward reaching
//		knee = foot + (knee - foot).normalize() * thighLenght;
//		hip = knee + (hip - knee).normalize() * shinLenght;
//	}
//
//	void Draw(SDL_Renderer* renderer)
//	{
//		// target circle
//		DrawCircle(renderer, target, 10.f, { 255, 255, 0, 255 });
//
//		// foot
//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//		DrawCircle(renderer, foot, 10.f, { 255, 0, 0, 255 });
//		DrawLine(renderer, foot, knee);
//
//		// knee
//		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
//		DrawCircle(renderer, knee, 10.f, { 0, 255, 0, 255 });
//		DrawLine(renderer, knee, hip);
//
//		// hip
//		DrawCircle(renderer, hip, 10.f, { 0, 0, 255, 255 });
//	}
//
//	void calculateNextStep(SDL_Renderer* renderer)
//	{
//		//Vec2 diff = target - foot;
//		//DrawLine(renderer, hip, hip + diff);
//
//		//float theta = atan2f(diff.y, diff.x);
//		//printf("%f\n", Rad2Deg(theta));
//
//		//Vec2 dir = { cosf(theta-M_PI/4), sinf(theta- M_PI / 4) };
//
//		//DrawLine(renderer, hip, hip - dir * thighLenght);
//
//		//DrawLine(renderer, foot, foot + dir * shinLenght);
//		//nextStep = foot + dir * shinLenght;
//
//		//DrawCircle(renderer, nextStep, 20, { 255, 0, 255, 255 });
//		//float radius = 60.f;
//
//		//float targetX = foot.x - radius * cosf(theta + M_PI/2);
//		//float targetY = foot.y - radius * sinf(theta + M_PI/2);
//		//Vec2 nextStep = { targetX, targetY };
//		//DrawPoint(renderer, nextStep, { 255, 255, 255, 255 });
//		//DrawCircle(renderer, nextStep, 20, { 255, 0, 255, 255 });
//
//		//if (diff.length() > 140)
//		//{
//		//	foot = nextStep;
//		//}
//
//		//float targetX = foot.x - radius * direction.x;
//		//float targetY = foot.y - radius * direction.y;
//
//	}
//};