#pragma once

#include "Component.h"
#include "Vector2.h"

class Collision;

class Position : public Component
{
private:
	Vector2 vector;
	Collision * collision;

public:
	~Position();

	bool Init();
	void PostInit();

	void Update(float gameTime);
	void Draw();

	Vector2 * Vector();

	float X() const;
	float Y() const;
	float Depth() const;

	void X(float value);
	void Y(float value);
	void Depth(float value);

	void Translate(float dx, float dy);
	void Translate(Vector2 vec);

	void MoveTo(float px, float py);
	void MoveTo(Vector2 vec);
};