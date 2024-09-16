#pragma once

struct Vec2
{
	float x{};
	float y{};

	inline Vec2() = default;

	Vec2(float scalar) : x(scalar), y(scalar)
	{}

	Vec2(float scalar_x, float scalar_y) : x(scalar_x), y(scalar_y)
	{}

	Vec2(int scalar_x, int scalar_y) : x((float)scalar_x), y((float)scalar_y)
	{}


	void operator+=(Vec2 b)
	{
		x += b.x;
		y += b.y;
	}
	void operator-=(Vec2 b)
	{
		x -= b.x;
		y -= b.y;
	}

	Vec2 operator*(float b) const
	{
		return { x * b, y * b };
	}
	Vec2 operator*(Vec2 b) const
	{
		return { x * b.x, y * b.y };
	}

	Vec2 operator/(float b) const
	{
		return { x / b, y / b };
	}

	Vec2 operator-(float b) const
	{
		return { x - b, y - b };
	}
	Vec2 operator-(Vec2 b) const
	{
		return { x - b.x, y - b.y };
	}
	Vec2 operator-() const
	{
		return { -x, -y };
	}

	Vec2 operator+(Vec2 b) const
	{
		return { x + b.x, y + b.y };
	}

	bool operator==(Vec2 b) const
	{
		return x == b.x && y == b.y;
	}

	float length()
	{
		return (float)sqrt(x * x + y * y);
	}

	Vec2 normalize()
	{
		float len = length();
		return { x / len, y / len };
	}

	//bool operator<(Vec2 b) const
	//{
	//	return x < b.x && y < b.y;
	//}

	//bool operator>(Vec2 b) const
	//{
	//	return x > b.x && y > b.y;
	//}

	//float dot(const Vec2& a, const Vec2& b)
	//{
	//	return a.x * b.x + a.y * b.y;
	//}
};


inline float dot(const Vec2& a, const Vec2& b)
{
	return (a.x * b.x + a.y * b.y);
}

inline float length(const Vec2& v)
{
	return (float)sqrt(dot(v, v));
}

inline Vec2 normalize(const Vec2& v)
{
	return (v / length(v));
}
