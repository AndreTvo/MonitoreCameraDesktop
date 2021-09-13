#include "Vector2.h"

const Vector2 Vector2::LOW_GRAVITY = Vector2(0, 4.905);
const Vector2 Vector2::GRAVITY = Vector2(0, 9.81);
const Vector2 Vector2::HIGH_GRAVITY = Vector2(0, 19.62);
const Vector2 Vector2::VERY_HIGH_GRAVITY = Vector2(0, 39.24);

const Vector2 Vector2::UP = Vector2(0, -1);
const Vector2 Vector2::DOWN = Vector2(0, 1);
const Vector2 Vector2::LEFT = Vector2(-1, 0);
const Vector2 Vector2::RIGHT = Vector2(1, 0);
const Vector2 Vector2::ZERO = Vector2(0, 0);

const Vector2 Vector2::OUT_OF_SCREEN = Vector2(-1, -1);
const Vector2 Vector2::NULL_VECTOR = Vector2(0, 0);

Vector2::Vector2()
{
	x = 0;
	y = 0;
	depth = 0.5f;
}

Vector2::Vector2(float x, float y, float depth) : x(x), y(y), depth(depth)
{
}

Vector2::~Vector2()
{
}

float Vector2::X() const
{
	return x;
}

float Vector2::Y() const
{
	return y;
}

float Vector2::Depth() const
{
	return depth;
}

void Vector2::X(float value)
{
	x = value;
}

void Vector2::Y(float value)
{
	y = value;
}

void Vector2::Depth(float value)
{
	depth = value;
}

void Vector2::Translate(float dx, float dy)
{
	x += dx;
	y += dy;
}

void Vector2::Translate(const Vector2 & vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2::MoveTo(float px, float py)
{
	x = px;
	y = py;
}

void Vector2::MoveTo(const Vector2 & vec)
{
	x = vec.x;
	y = vec.y;
}

float Vector2::Magnitude() const
{
	return sqrt(x*x + y*y);
}

void Vector2::Normalize()
{
	float mag = Magnitude();

	if (mag > 0)
	{
		(*this) *= ((float)1) / mag;
	}
}

Vector2 Vector2::Unit() const
{
	Vector2 result = *this;
	result.Normalize();

	return result;
}

void Vector2::Clear()
{
	x = y = 0;
}

void Vector2::Invert()
{
	x = -x;
	y = -y;
}

float Vector2::operator[](unsigned i) const
{
	if (i == 0) 
		return x;

	return y;
}

float & Vector2::operator[](unsigned i)
{
	if (i == 0)
		return x;

	return y;
}

Vector2 Vector2::operator+(const Vector2 & other) const
{
	Vector2 result = *this;
	result += other;
	return result;
}

Vector2 Vector2::operator-(const Vector2 & other) const
{
	Vector2 result = *this;
	result -= other;
	return result;
}

Vector2 & Vector2::operator+=(const Vector2 & other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2 Vector2::operator*(const int & other) const
{
	Vector2 result = *this;
	result *= other;
	return result;
}

Vector2 & Vector2::operator*=(const int & other)
{
	x *= other;
	y *= other;

	return *this;
}

Vector2 Vector2::operator*(const float & other) const
{
	Vector2 result = *this;
	result *= other;
	return result;
}

Vector2 & Vector2::operator*=(const float & other)
{
	x *= other;
	y *= other;

	return *this;
}

bool Vector2::operator==(const Vector2 & other) const
{
	int actualX = x;
	int actualY = y;

	int otherX = other.x;
	int otherY = other.y;

	if (actualX == otherX)
		if (actualY == otherY)
				return true;

	return false;
}