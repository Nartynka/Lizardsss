#include "Worm.h"
#include <cassert>

Worm::Worm(SDL_Renderer* renderer)
{
	Vec2 origin = { 500.f, 100.f };
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

	// only for debug
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

	for (int i = 0; i < numLegs; ++i)
	{
		legs.emplace_back();
	}

	GenerateBodyPoints();
}


Worm::Worm(SDL_Renderer* renderer, WormOptions::Options options)
{
	Vec2 origin = { 500.f, 100.f };
	numParticles = options.numParticles;
	numConstrains = numParticles - 1;
	outlineColor = options.outlineColor;
	faceColor = options.faceColor;
	legsColor = options.legsColor;
	faceType = options.faceType;
	eyesType = options.eyesType;
	hasFace = options.hasFace;
	hasEyes = options.hasEyes;
	hasLegs = options.hasLegs;

	for (int i = 0; i < numParticles; ++i)
	{
		Particle p;
		p.pos = origin;
		p.radius = options.radius;
		origin -= Vec2(p.radius, 0.f);
		p.vel = Vec2(0.f);
		particles.push_back(p);
	}

	particles[0].mass = 0.f;

	// only for debug
	particles[0].color = { 200, 50, 150 };

	for (int i = 0; i < numConstrains; ++i)
	{
		ElasticDistance c;

		c.idx_a = i;
		c.idx_b = i + 1;

		if (options.hasAutoDistance)
		{
			Vec2 diff = particles[i].pos - particles[i + 1].pos;
			c.distance = length(diff);
		}
		else
		{
			c.distance = options.distance;
		}

		constrains.push_back(c);
	}

	SDL_Surface* eye_surf = IMG_Load(options.GetEyesPath(eyesType));
	SDL_Texture* eye_text = SDL_CreateTextureFromSurface(renderer, eye_surf);
	eyes.first = { eye_text, {0}, { eye_surf->h / 2, eye_surf->w / 2} };
	eyes.second = { eye_text, {0}, { eye_surf->h / 2, eye_surf->w / 2} };
	SDL_FreeSurface(eye_surf);

	for (int i = 0; i < numLegs; ++i)
	{
		legs.emplace_back();
	}

	GenerateBodyPoints();
}

Worm::~Worm()
{
	SDL_DestroyTexture(eyes.first.text);
	SDL_DestroyTexture(eyes.second.text);
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


void Worm::MoveLegs(SDL_Renderer* renderer)
{
	int spacing = points_l.size() / (numLegs % 2 == 0 ? numLegs : numLegs + 1);
	int idx = spacing;
	for (int i = 0; i < numLegs; ++i)
	{
		Vec2 dir;
		if (i % 2 != 0)
		{
			legs[i].SetAnchor(points_r[idx]);
			dir = (points_r[idx] - points_r[idx + 1]).normalize();
			legs[i].CalculateNextStep(renderer, dir);
			idx += spacing * 2;
		}
		else
		{
			//if (idx > points_l.size()-1)
			//	idx = idx - 15;
			legs[i].SetAnchor(points_l[idx]);
			dir = (points_l[idx] - points_l[idx + 1]).normalize();
			legs[i].CalculateNextStep(renderer, dir, false);
		}

		legs[i].FABRIK();
	}

	{
	//legs[0].SetAnchor(points_l[idx]);
	//legs[1].SetAnchor(points_r[idx]);
	//legs[2].SetAnchor(points_l[idx * 3]);
	//legs[3].SetAnchor(points_r[idx * 3]);

	//Vec2 dir = (points_l[idx] - points_l[idx + 1]).normalize();
	//legs[0].CalculateNextStep(renderer, dir);
	//legs[0].FABRIK();
	//legs[0].Draw(renderer);

	//dir = (points_r[idx] - points_r[idx + 1]).normalize();
	//legs[1].CalculateNextStep(renderer, dir);
	//legs[1].FABRIK();
	//legs[1].Draw(renderer);

	//dir = (points_l[idx * 3] - points_l[idx * 3 + 1]).normalize();
	//legs[2].CalculateNextStep(renderer, dir);
	//legs[2].FABRIK();
	//legs[2].Draw(renderer);

	//dir = (points_r[idx * 3] - points_r[idx * 3 + 1]).normalize();
	//legs[3].CalculateNextStep(renderer, dir);
	//legs[3].FABRIK();
	//legs[3].Draw(renderer);
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

// Generates points along a rounded line around the center
// default is half circle (180 deg. or pi) pointing outside the center. Like a simile :) or closing bracket ")"
std::vector<Vec2> Worm::GenerateRoundedLines(const Vec2& center, float radius, float startAngle, float endAngle, int direction)
{
	std::vector<Vec2> points;
	int segments = 10; // Number of segments to divide the semi-circle - more segments, more smooth circle
	float angleStep = endAngle / segments; // Step size for each segment, each segment will have even step

	for (int i = 0; i <= segments; ++i)
	{
		float angle = startAngle + i * angleStep;
		float x = center.x + radius * cosf(angle + direction * M_PI / 2);
		float y = center.y + radius * sinf(angle + direction * M_PI / 2);
		points.emplace_back(x, y);
	}
	return points;
}



void Worm::DrawBody(SDL_Renderer* renderer, bool shouldDrawLegs)
{
	/// Side lines
	GenerateBodyPoints();
	//SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	// line from left points
	std::vector<SDL_Point> sdl_points_left = Vec2ToSDLPoints(points_l);
	SDL_RenderDrawLines(renderer, &sdl_points_left[0], sdl_points_left.size());

	// line from right points
	std::vector<SDL_Point> sdl_points_right = Vec2ToSDLPoints(points_r);
	SDL_RenderDrawLines(renderer, &sdl_points_right[0], sdl_points_right.size());


	/// Rounded Ends
	std::vector<Vec2> front_circle = GenerateRoundedLines(particles[0].pos, particles[0].radius, thetas[0]);
	std::vector<Vec2> back_circle = GenerateRoundedLines(particles[numParticles - 1].pos, particles[numParticles - 1].radius, thetas[thetas.size() - 1], M_PI, -1);
	std::vector<SDL_Point> sdl_front_points = Vec2ToSDLPoints(front_circle);
	std::vector<SDL_Point> sdl_back_points = Vec2ToSDLPoints(back_circle);
	SDL_RenderDrawLines(renderer, &sdl_front_points[0], sdl_front_points.size());
	SDL_RenderDrawLines(renderer, &sdl_back_points[0], sdl_back_points.size());

	/// Eyes & smile :D
	if(hasFace)
		DrawFace(renderer);
	if (hasEyes)
		DrawEyes(renderer);

	/// Legs
	if (hasLegs && shouldDrawLegs)
	{
		MoveLegs(renderer);
		DrawLegs(renderer);
	}
}

void Worm::DrawLegs(SDL_Renderer* renderer)
{
	float radius = 10.f;
	SDL_SetRenderDrawColor(renderer, legsColor.r, legsColor.g, legsColor.b, legsColor.a);
	for (int i = 0; i < numLegs; ++i)
	{
		const Leg& leg = legs[i];

		Vec2 direction;

		if (i < numLegs - 1)
		{
			direction = legs[i + 1].foot - leg.foot;
		}
		else if(numLegs > 1)
		{
			direction = leg.foot - legs[i - 1].foot;
		}

		float theta = atan2f(direction.y, direction.x);

		// hip
		std::pair<Vec2, Vec2> hips = GenerateSidesPoints(leg.hip, radius, theta);
		// knee
		std::pair<Vec2, Vec2> knees = GenerateSidesPoints(leg.knee, radius, theta);
		// foot
		std::pair<Vec2, Vec2> foots = GenerateSidesPoints(leg.foot, radius, theta);

		std::vector<Vec2> points_l = { hips.first, knees.first, foots.first };
		std::vector<Vec2> points_r = { hips.second, knees.second, foots.second };

		// line from left points
		std::vector<SDL_Point> sdl_points_left = Vec2ToSDLPoints(points_l);
		SDL_RenderDrawLines(renderer, &sdl_points_left[0], sdl_points_left.size());

		// line from right points
		std::vector<SDL_Point> sdl_points_right = Vec2ToSDLPoints(points_r);
		SDL_RenderDrawLines(renderer, &sdl_points_right[0], sdl_points_right.size());


		/// Rounded Ends
		std::vector<Vec2> leg_end;
		// idk why the last leg has to be inverted :/
		if (numLegs > 1 && i == numLegs-1)
		{
			leg_end = GenerateRoundedLines(leg.foot, radius, theta, M_PI, -1);
		}
		else
		{
			leg_end = GenerateRoundedLines(leg.foot, radius, theta);
		}
		std::vector<SDL_Point> leg_end_points = Vec2ToSDLPoints(leg_end);
		SDL_RenderDrawLines(renderer, &leg_end_points[0], leg_end_points.size());
	}
}

void Worm::DrawEyes(SDL_Renderer* renderer)
{
	const Vec2& headPos = particles[0].pos;
	float r = particles[0].radius;

	float leftAngle = thetas[0] + M_PI / 2;
	float rightAngle = thetas[0] - M_PI / 2;

	float leftX = headPos.x - r * cosf(leftAngle);
	float leftY = headPos.y - r * sinf(leftAngle);

	float rightX = headPos.x - r * cosf(rightAngle);
	float rightY = headPos.y - r * sinf(rightAngle);

	eyes.first.pos = { leftX, leftY };
	eyes.second.pos = { rightX, rightY };

	DrawPoint(renderer, eyes.first);
	DrawPoint(renderer, eyes.second);
	//DrawPoint(renderer, { rightX, rightY }, { 255, 255, 255, 255 });
	//DrawPoint(renderer, { leftX, leftY }, { 255, 255, 255, 255 });
}

void Worm::DrawFace(SDL_Renderer* renderer)
{
	Vec2 headPos = particles[0].pos;

	int direction = faceType == WormOptions::Sad ? -1 : 1;
	
	float endAngle = Deg2Rad(120.f);
	float startAngle = thetas[0]+(M_PI - endAngle)/2; // (180 - 120) / 2
	float radius = particles[0].radius - 25.f;

	if (faceType == WormOptions::Sad)
	{
		radius = particles[0].radius - 5.f;
	}
	if (faceType == WormOptions::WideSimle)
	{
		startAngle = thetas[0];
		endAngle = M_PI;
	}

	float x = headPos.x - radius * cosf(thetas[0]);
	float y = headPos.y - radius * sinf(thetas[0]);

	std::vector<Vec2> smile = GenerateRoundedLines({x, y}, 15.f, startAngle, endAngle, direction);
	std::vector<SDL_Point> p = Vec2ToSDLPoints(smile);

	SDL_SetRenderDrawColor(renderer, faceColor.r, faceColor.g, faceColor.b, faceColor.a);
	SDL_RenderDrawLines(renderer, &p[0], p.size());

	if(faceType == WormOptions::WideSimle)
		SDL_RenderDrawLine(renderer, p[0].x, p[0].y, p[p.size() - 1].x, p[p.size() - 1].y);

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderDrawPoint(renderer, x, y);
}


void Worm::DrawDebugBody(SDL_Renderer* renderer)
{
	GenerateBodyPoints();
	for (const Particle& p : particles)
	{
		DrawDebugParticle(renderer, p.pos, p.radius, p.color.SDLColor());
	}

	DrawDebugSidePoints(renderer);
}


void Worm::DrawDebugLegs(SDL_Renderer* renderer)
{
	if (hasLegs)
	{
		MoveLegs(renderer);
		for (const Leg& leg : legs)
		{
			leg.DrawDebug(renderer);
		}
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
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

void Worm::MoveToRandom(const float dt)
{
	Particle& head = particles[0];

	Vec2 diff = { target.x - head.pos.x, target.y - head.pos.y };
	float distance = length(diff);

	if (distance <= 2.f || target == Vec2(0.f, 0.f))
	{
		do 
		{
			target = { RandomInRange(-10, 1090), RandomInRange(-10, 730) };
		} while (length(target - head.pos) < 200);
	}
	else 
	{
		Vec2 dir = normalize(diff);
		head.vel = dir * speed * dt;
		head.pos += head.vel;
	}
}
