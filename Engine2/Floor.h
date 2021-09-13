#pragma once

#include "Object.h"
#include "FloorHolder.h"

class Floor : public Object
{
private:
	static int refcount;

public:
	static void ResetIdRef();

private:
	int idRef;
	Rect * rect;
	Collision * collision;
	bool passthrough;
	bool elevator;

	float fixedX;
	float fixedY;

public:
	Floor(float height, float width, float posX, float posY, bool passThrough);
	~Floor();

	void Update(float gameTime);
	void Draw();

	int IdRef() const;

	float Height() const;
	float Width() const;
	void Height(float value);
	void Width(float value);

	void Passthrough(bool value);
	bool Passthrough() const;

	void Elevator(bool value);
	bool Elevator() const;

	void FixedX(float value);
	void FixedY(float value);

	float FixedX() const;
	float FixedY() const;
};