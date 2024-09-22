#pragma once

#include <vector>
#include "Vec2.h"
#include "Utils.h"
#include "MyUI.h"
#include "Leg.h"

struct Particle
{
	Vec2 pos; // center
	Vec2 prevPos;
	Vec2 vel;
	float radius = 30.f;
	float mass = 1.f;
	Color color = { 55, 150, 100, 255 }; // only for debug
};

struct ElasticDistance
{
	int idx_a = -1;
	int idx_b = -1;

	float distance = 5.f;
	//float stiffness = 0.1f;
	float compliance = 0.03f;
};

struct Worm
{
	int numParticles = 72;
	std::vector<Particle> particles;

	int numConstrains = numParticles - 1;
	std::vector<ElasticDistance> constrains;

	SDL_Color outlineColor = { 255, 255, 255, 255 };
	SDL_Color faceColor = { 255, 255, 255, 255 };
	SDL_Color legsColor = { 230, 230, 230, 255 };

	WormOptions::Face faceType;
	WormOptions::Eyes eyesType;

	bool hasEyes = true;
	bool hasFace = true;
	bool hasLegs = true;

	int numLegs = 4;
	std::vector<Leg> legs;

private:
	std::vector<Vec2> moveToPoints;
	const float speed = 120.f;
	Vec2 target = {0.f, 0.f};

	// only for debug
	std::vector<Vec2> points_r;
	std::vector<Vec2> points_l;
	std::vector<Vec2> points_f;
	// only first and last is used - rest is only for debug
	std::vector<float> thetas;

	void GenerateBodyPoints();
	void MoveLegs(SDL_Renderer* renderer);
	std::pair<Vec2, Vec2> GenerateSidesPoints(const Vec2& center, float radius, float angle);
	std::vector<Vec2> GenerateRoundedLines(const Vec2& center, float radius, float startAngle, float endAngle = M_PI, int direction = 1);

	std::pair<Point, Point> eyes;

	void DrawEyes(SDL_Renderer* renderer);
	void DrawFace(SDL_Renderer* renderer);
	void DrawLegs(SDL_Renderer* renderer);

public:
	Worm(SDL_Renderer* renderer);
	Worm(SDL_Renderer* renderer, WormOptions::Options options);

	~Worm();

	void ResolveConstrains();

	void DrawBody(SDL_Renderer* renderer, bool shouldDrawLegs=true);

	void DrawDebugBody(SDL_Renderer* renderer);
	void DrawDebugLegs(SDL_Renderer* renderer);
	void DrawDebugSidePoints(SDL_Renderer* renderer);

	void AddMoveToPoint(Vec2 pos);
	void MoveTowards(const double dt);

	void MoveToRandom(const float dt);

};