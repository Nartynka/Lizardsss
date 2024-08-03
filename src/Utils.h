#pragma once

#include <SDL.h>
#include <vector>
#include "Vec2.h"
#include "SDL_image.h"


struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color(float scalar) : r(scalar), g(scalar), b(scalar), a(scalar)
	{}

	Color(float s_r, float s_g, float s_b, float s_a = 1.f) : r(s_r), g(s_g), b(s_b), a(s_a)
	{}

	SDL_Color SDLColor() const
	{
		SDL_Color c = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
		return c;
	}
};

struct Point
{
	SDL_Texture* text = nullptr;
	Vec2 pos;
	Vec2 size;
};

inline void DrawCircle(SDL_Renderer* renderer, Vec2 center, int radius, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	int x0 = (int)center.x;
	int y0 = (int)center.y;

	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int error = dx - (radius << 1);

	while (x >= y) {
		SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

		if (error <= 0) {
			y++;
			error += dy;
			dy += 2;
		}
		if (error > 0) {
			x--;
			dx += 2;
			error += dx - (radius << 1);
		}
	}
}

inline void DrawPoint(SDL_Renderer* renderer, Vec2 center, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer, (int)center.x, (int)center.y);
}

inline void DrawPoint(SDL_Renderer* renderer, Point p)
{
	SDL_Rect dsrect = { (int)(p.pos.x-(p.size.x/2)), (int)(p.pos.y- (p.size.y / 2)), (int)p.size.x, (int)p.size.y };
	SDL_RenderCopy(renderer, p.text, nullptr, &dsrect);
}

inline Point CreatePoint(SDL_Renderer* renderer, Vec2 center)
{
	SDL_Surface* d_surf = IMG_Load("res/dot.png");
	SDL_Texture* d_text = SDL_CreateTextureFromSurface(renderer, d_surf);
	Point p = { d_text, center, { d_surf->h, d_surf->w} };
	SDL_FreeSurface(d_surf);
	return p;
}

inline void DrawDebugParticle(SDL_Renderer* renderer, Vec2 center, float radius, SDL_Color color)
{
	DrawCircle(renderer, center, (int)radius, color);
	DrawPoint(renderer, center, color);
}

inline Vec2 ConstrainDistance(Vec2 point, Vec2 anchor, float distance) 
{
	Vec2 diff = point - anchor;
	Vec2 dir = normalize(diff);
	Vec2 v = dir * distance;
	return v + anchor;
}

inline std::vector<SDL_Point> Vec2ToSDLPoints(std::vector<Vec2> points)
{
	std::vector<SDL_Point> sdl_points;
	for (const Vec2& point : points)
	{
		sdl_points.push_back({ (int)point.x, (int)point.y });
	}
	return sdl_points;
}