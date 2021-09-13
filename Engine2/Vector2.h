#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

class Vector2
{
private:
	float x, y, depth;

public:
	Vector2();
	Vector2(float x, float y, float depth = 0.5f);
	~Vector2();

	float X() const;
	float Y() const;
	float Depth() const;

	void X(float value);
	void Y(float value);
	void Depth(float value);

	void Translate(float dx, float dy);
	void Translate(const Vector2 & vec);

	void MoveTo(float px, float py);
	void MoveTo(const Vector2 & vec);

	const static Vector2 LOW_GRAVITY;
	const static Vector2 GRAVITY;
	const static Vector2 HIGH_GRAVITY;
	const static Vector2 VERY_HIGH_GRAVITY;

	const static Vector2 UP;
	const static Vector2 DOWN;
	const static Vector2 LEFT;
	const static Vector2 RIGHT;
	const static Vector2 ZERO;

	const static Vector2 OUT_OF_SCREEN;
	const static Vector2 NULL_VECTOR;

	float Magnitude() const;
	void Normalize();
	Vector2 Unit() const;

	void Clear();
	void Invert();

	float operator[](unsigned i) const;
	float& operator[](unsigned i);

	// operações com vetor
	Vector2 operator+ (const Vector2 & other) const;
	Vector2 operator- (const Vector2 & other) const;
	Vector2 & operator+= (const Vector2 & other);
	Vector2 & operator-= (const Vector2 & other);

	Vector2 operator* (const int &other) const;
	Vector2 & operator*= (const int &other);

	Vector2 operator* (const float &other) const;
	Vector2 & operator*= (const float &other);

	bool operator== (const Vector2 & other) const;
};