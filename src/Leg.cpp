#include "Leg.h"

//Leg::Leg(const Vec2& initialHipPosition) : anchor(initialHipPosition) {}

#define FABRIK_DEBUG false

Leg::Leg()
{
	stepLenght = (shinLenght + thighLenght) / 2;
}


void Leg::SetAnchor(const Vec2& newAnchor)
{
	anchor = newAnchor;
}

void Leg::FABRIK()
{
	hip = anchor;

	// forward reaching
	for (int i = 0; i < 10; ++i)
	{
		knee = hip + (knee - hip).normalize() * shinLenght;
		// backward reaching
		knee = foot + (knee - foot).normalize() * thighLenght;
	}

	//hip = knee + (hip - knee).normalize() * shinLenght;
	//Vec2 diff = knee - hip;
	//float th = atan2f(diff.y, diff.x);
	//if (i == 2)
	//{
	//	th += M_PI / 2;
	//	//DrawCircle(renderer, { x, y }, 10, { 255, 255, 0, 255 });
	//}
	//else if (i == 3) // back right leg
	//{
	//	th -= M_PI / 2;
	//	//DrawCircle(renderer, { x, y }, 10, { 255, 255, 0, 255 });
	//}
	//float r = shinLenght;
	//float x = hip.x + r * cosf(th);
	//float y = hip.y + r * sinf(th);
	//knee = { x, y };
}

void Leg::DrawDebug(SDL_Renderer* renderer) const
{
	// target circle
	//DrawCircle(renderer, anchor, 10.f, { 255, 255, 0, 255 });

	//Vec2 diff = foot - hip;
	//Vec2 v = { -diff.y, diff.x };
	//Vec2 knee = this->knee;
	//if (i == 2) // back left leg
	//{
	//	//DrawLine(renderer, knee, knee + v, {255, 0, 255, 255});
	//	//DrawCircle(renderer, knee + v, 10, { 255, 0, 255, 255 });
	//	knee += v;
	//	//this->FABRIK();
	//} else if (i == 3) // back right leg
	//{
	//	//DrawLine(renderer, knee, knee - v, { 255, 255, 0, 255 });
	//	//DrawCircle(renderer, knee - v, 10, { 255, 255, 0, 255 });
	//	knee -= v;
	//	//this->FABRIK();
	//}


	// foot - red circle
	DrawCircle(renderer, foot, 10, { 255, 0, 0, 255 });
	DrawLine(renderer, foot, knee, { 255, 0, 0, 255 });

	// knee - green circle
	DrawCircle(renderer, knee, 10, { 0, 255, 0, 255 });
	DrawLine(renderer, knee, hip, { 0, 255, 0, 255 });

	// hip - blue circle, attached to body
	DrawCircle(renderer, hip, 10, { 0, 0, 255, 255 });
}

void Leg::CalculateNextStep(SDL_Renderer* renderer, Vec2 dir, bool isRightSide)
{
	float theta = atan2f(dir.y, dir.x) + (float)(isRightSide ? M_PI / 2 : -M_PI / 2);
	float targetX = hip.x + stepLenght * cosf(theta);
	float targetY = hip.y + stepLenght * sinf(theta);

	Vec2 nextStep = { targetX, targetY };

#if FABRIK_DEBUG
	// Yellow forward vector from foot
	DrawLine(renderer, foot, foot + (dir * 50), { 255, 255, 0, 255});
	DrawCircle(renderer, nextStep, 10, { 255, 0, 255, 255 });
#endif // DEBUG


	Vec2 diff = foot - hip;

	Vec2 a = knee - hip;
	Vec2 b = foot - knee;
	// a • b = 1 * 1 * cos(theta)
	float cos = -dot(a.normalize(), b.normalize());
	if (diff.length() > stepLenght * 2 || Rad2Deg(acosf(cos) > 130))
	{
		foot = nextStep;
	}

	//printf("%f %f\n", Rad2Deg(acos(cos)), acos(cos)/M_PI);
}

void Leg::CalculateNextStep(Vec2 dir, bool isRightSide)
{
	float theta = atan2f(dir.y, dir.x) + (float)(isRightSide ? M_PI / 2 : -M_PI / 2);
	float targetX = hip.x + stepLenght * cosf(theta);
	float targetY = hip.y + stepLenght * sinf(theta);

	//float targetX = hip.x - stepLenght * dir.x;
	//float targetY = hip.y - stepLenght * dir.y;
	Vec2 nextStep = { targetX, targetY };

	Vec2 diff = foot - hip;
	if (diff.length() > stepLenght*2)
	{
		foot = nextStep;
	}
}