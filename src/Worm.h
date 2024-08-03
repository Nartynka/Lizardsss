#pragma once

#include <vector>
#include "Vec2.h"
#include "Utils.h"

struct Particle
{
	Vec2 pos; // center
	Vec2 prevPos;
	Vec2 vel;
	float radius = 30.f;
	float mass = 1.f;
	Color color = { 55, 150, 100, 255 };
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
	std::vector<Particle> particles;
	const int numParticles = 128;

	const int numConstrains = numParticles - 1;
	std::vector<ElasticDistance> constrains;

	Color outlineColor = { 255, 255, 255, 255 };
	Color faceColor = { 255, 255, 255, 255 };
private:
	std::vector<Vec2> moveToPoints;
	const float speed = 1000.f;

	std::vector<Vec2> points_r;
	std::vector<Vec2> points_l;
	std::vector<Vec2> points_f;
	std::vector<float> thetas;

	void GenerateBodyPoints();
	std::pair<Vec2, Vec2> GenerateSidesPoints(const Vec2& center, float radius, float angle);
	std::vector<Vec2> GenerateRoundedEnds(const Vec2& center, float radius, float startAngle, int direction = 1);

	std::pair<Point, Point> eyes;

	void DrawFace(SDL_Renderer* renderer);
public:
	Worm(SDL_Renderer* renderer);

	void ResolveConstrains();

	void DrawBody(SDL_Renderer* renderer);


	void DrawDebugBody(SDL_Renderer* renderer);
	void DrawDebugSidePoints(SDL_Renderer* renderer);

	void AddMoveToPoint(Vec2 pos);
	void MoveTowards(const double dt);

};