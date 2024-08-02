#include "Worm.h"


Worm::Worm(SDL_Renderer* renderer)
{
	Vec2 origin = { 50.f, 100.f };
	int radius = 30;
	int x = radius / numParticles;
	for (int i = 0; i < numParticles; ++i)
	{
		Particle p;
		p.pos = origin;
		//p.radius = rand() % radius + 30-i;
		//radius -= x;
		origin -= Vec2(p.radius, 0.f);
		p.vel = Vec2(0.f);
		particles.push_back(p);
	}

	particles[0].mass = 0.f;
	particles[0].color = { 200, 50, 150 };

	for (int i = 0; i < numConstrains; ++i)
	{
		ElasticDistance c;

		c.idx_a = i;
		c.idx_b = i + 1;

		Vec2 diff = particles[i].pos - particles[i + 1].pos;
		//c.Distance = length(diff);

		constrains.push_back(c);
	}

	SDL_Surface* eye_surf = IMG_Load("res/dotw.png");
	SDL_Texture* eye_text = SDL_CreateTextureFromSurface(renderer, eye_surf);
	eyes.first = { eye_text, {0}, {eye_surf->h / 2, eye_surf->w / 2} };
	eyes.second = { eye_text, {0}, { eye_surf->h / 2, eye_surf->w / 2} };
	SDL_FreeSurface(eye_surf);

	GenerateBodyPoints();
}

void Worm::ResolveConstrains()
{
	for (const ElasticDistance& c : constrains)
	{
		const Particle& p1 = particles[c.idx_a];
		Particle& p2 = particles[c.idx_b];

		p2.pos = ConstrainDistance(p2.pos, p1.pos, c.distance);
	}
}


void Worm::GenerateBodyPoints()
{
	points_f.clear();
	points_l.clear();
	points_r.clear();
	thetas.clear();

	for (int i = 0; i < numParticles; ++i)
	{
		const Particle& p = particles[i];

		Vec2 direction;

		if (i < numParticles - 1)
		{
			direction = particles[i + 1].pos - p.pos;
		}
		else
		{
			direction = p.pos - particles[i - 1].pos;
		}

		float theta = atan2f(direction.y, direction.x);
		thetas.push_back(theta);

		// front
		float x = p.pos.x - p.radius * cosf(theta);
		float y = p.pos.y - p.radius * sinf(theta);
		Vec2 front = { x, y };

		std::pair<Vec2, Vec2> pair = GenerateSidesPoints(p.pos, p.radius, theta);
		points_f.push_back(front);
		points_l.push_back(pair.first);
		points_r.push_back(pair.second);
	}
}


// returns points on the left and right side of the particle
std::pair<Vec2, Vec2> Worm::GenerateSidesPoints(const Vec2& center, float radius, float angle)
{
	// We subtract so the point is at the back of the circle
	// and if we add (clockwise) pi/2 (90 degrees) we have the right side, if we sub. pi/2 we have the left side
	float left_x = center.x - radius * cosf(angle - M_PI / 2);
	float left_y = center.y - radius * sinf(angle - M_PI / 2);
	float right_x = center.x - radius * cosf(angle + M_PI / 2);
	float right_y = center.y - radius * sinf(angle + M_PI / 2);
	return { {left_x, left_y}, {right_x, right_y} };
}


std::vector<Vec2> Worm::GenerateRoundedEnds(const Vec2& center, float radius, float startAngle, int direction)
{
	std::vector<Vec2> points;
	int segments = 10; // Number of segments to divide the semi-circle - more segments, more smooth circle
	float angleStep = M_PI / segments; // Step size for each segment, each segment will have even step

	for (int i = 0; i <= segments; ++i)
	{
		float angle = startAngle + i * angleStep;
		float x = center.x + radius * cosf(angle + direction * M_PI / 2);
		float y = center.y + radius * sinf(angle + direction * M_PI / 2);
		points.emplace_back(x, y);
	}
	return points;
}

float normalize_angle(float angle) 
{
	while (angle < 0) angle += 2 * M_PI;
	while (angle >= 2 * M_PI) angle -= 2 * M_PI;
	return angle;
}

void Worm::DrawFace(SDL_Renderer* renderer)
{
	const Vec2 headPos = particles[0].pos;
	float r = particles[0].radius;

	float leftAngle = normalize_angle(thetas[0] + M_PI / 2);
	float rightAngle = normalize_angle(thetas[0] - M_PI / 2);

	float leftX = headPos.x - r * cosf(leftAngle);
	float leftY = headPos.y - r * sinf(leftAngle);

	float rightX = headPos.x - r * cosf(rightAngle);
	float rightY = headPos.y - r * sinf(rightAngle);

	eyes.first.pos = { leftX, leftY };
	eyes.second.pos = { rightX, rightY };

	DrawPoint(renderer, eyes.first);
	DrawPoint(renderer, eyes.second);


	std::vector<Vec2> smile = GenerateRoundedEnds(headPos, 10.f, thetas[0]);
	std::vector<SDL_Point> p = Vec2ToSDLPoints(smile);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLines(renderer, &p[0], p.size());
}

void Worm::DrawBody(SDL_Renderer* renderer)
{
	/// Side lines
	GenerateBodyPoints();
	// line from left points - green
	std::vector<SDL_Point> sdl_points_left = Vec2ToSDLPoints(points_l);
	SDL_SetRenderDrawColor(renderer, 0, 150, 50, 255);
	SDL_RenderDrawLines(renderer, &sdl_points_left[0], sdl_points_left.size());

	// line from right points - red
	std::vector<SDL_Point> sdl_points_right = Vec2ToSDLPoints(points_r);
	SDL_SetRenderDrawColor(renderer, 150, 0, 50, 255);
	SDL_RenderDrawLines(renderer, &sdl_points_right[0], sdl_points_right.size());


	/// Rounded Ends
	std::vector<Vec2> front_circle = GenerateRoundedEnds(particles[0].pos, particles[0].radius, thetas[0]);
	std::vector<Vec2> back_circle = GenerateRoundedEnds(particles[numParticles - 1].pos, particles[numParticles - 1].radius, thetas[thetas.size() - 1], -1);
	std::vector<SDL_Point> sdl_front_points = Vec2ToSDLPoints(front_circle);
	std::vector<SDL_Point> sdl_back_points = Vec2ToSDLPoints(back_circle);
	SDL_RenderDrawLines(renderer, &sdl_front_points[0], sdl_front_points.size());
	SDL_RenderDrawLines(renderer, &sdl_back_points[0], sdl_back_points.size());

	/// Eyes & smile :D
	DrawFace(renderer);

}


void Worm::DrawDebugBody(SDL_Renderer* renderer)
{
	for (const Particle& p : particles)
	{
		DrawDebugParticle(renderer, p.pos, p.radius, p.color.SDLColor());
	}
}

// Debug points for sides of the worm
void Worm::DrawDebugSidePoints(SDL_Renderer* renderer)
{
	for (int i = 0; i < points_l.size(); ++i)
	{
		Vec2 pl = points_l[i];
		Vec2 pr = points_r[i];
		Vec2 pf = points_f[i];
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(renderer, pl.x, pl.y);
		SDL_RenderDrawPoint(renderer, pr.x, pr.y);
		SDL_RenderDrawPoint(renderer, pf.x, pf.y);
	}
}


void Worm::AddMoveToPoint(Vec2 pos)
{
	moveToPoints.push_back(pos);
}

void Worm::MoveTowards(const double dt)
{
	if (moveToPoints.empty())
	{
		return;
	}

	Vec2 target = moveToPoints[0];
	Particle& head = particles[0];

	Vec2 diff = { target.x - head.pos.x, target.y - head.pos.y };
	float distance = length(diff);

	if (distance <= 2.f)
	{
		moveToPoints.erase(moveToPoints.begin());
	}
	else
	{
		Vec2 dir = normalize(diff);
		head.vel = dir * speed * (float)dt;
		head.pos += head.vel;
	}
}