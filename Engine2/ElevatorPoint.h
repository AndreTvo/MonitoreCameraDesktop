#pragma once

#include "Object.h"
#include "Vector2.h"

class ElevatorPoint : public Object
{
private:
	static int refcount;
	const static Vector2 moveUp;
	const static Vector2 moveDown;
	const static Vector2 moveLeft;
	const static Vector2 moveRight;

public:
	static void ResetIdRef();

private:
	int idRef;
	Rect * rect;
	Collision * collision;

	int directionOp;
	Vector2 direction;

public:
	ElevatorPoint(float height, float width, float posX, float posY, int direction);
	~ElevatorPoint();

	void Update(float gameTime);
	void Draw();

	int IdRef() const;
	
	float Height() const;
	float Width() const;
	Vector2 Direction() const;
	int DirectionOption() const;

	void Height(float value);
	void Width(float value);
	void Direction(int direction);

};
