#pragma once

#include <SDL.h>
#include "Vec2.h"
#include "Utils.h"

struct Leg
{
	float thighLenght = 45.f;
	float shinLenght = 35.f;
	float stepLenght; // "radius"

	Vec2 foot = { 0.f, 0.f }; // red circle
	Vec2 knee = { foot.x, foot.y + thighLenght }; // green circle
	Vec2 hip = { knee.x, knee.y + shinLenght }; // blue circle, attached to body

	Vec2 anchor;

	Leg();
	// Leg(const Vec2& initialHipPosition);
	void SetAnchor(const Vec2& newTarget);
	void FABRIK();
	void DrawDebug(SDL_Renderer* renderer) const;
	void CalculateNextStep(SDL_Renderer* renderer, Vec2 dir={0.f}, bool isRightSide = true);
	void CalculateNextStep(Vec2 dir, bool isRightSide = true);
};