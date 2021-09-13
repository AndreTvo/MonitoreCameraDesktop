#include "ElevatorPoint.h"

int ElevatorPoint::refcount = 0;
const Vector2 ElevatorPoint::moveUp = Vector2(0, -30);
const Vector2 ElevatorPoint::moveDown = Vector2(0, 30);

const Vector2 ElevatorPoint::moveLeft = Vector2(-30, 0);
const Vector2 ElevatorPoint::moveRight = Vector2(30, 0);

void ElevatorPoint::ResetIdRef()
{
	refcount = 0;
}

ElevatorPoint::ElevatorPoint(float height, float width, float posX, float posY, int direction)
{
	idRef = refcount++;
	position->MoveTo(posX, posY);

	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(rect = new Rect(height, width));

	directionOp = direction;

	switch (direction)
	{
		// (0 = UP, 1 = LEFT, 2 = RIGHT, 3 = DOWN, 4 = STOP)
		case 0:
			this->direction = moveUp;
			break;

		case 1:
			this->direction = moveLeft;
			break;

		case 2:
			this->direction = moveRight;
			break;

		case 3:
			this->direction = moveDown;
			break;

		case 4:
			this->direction = Vector2(0, 0);
			break;
	}

	type = ELEVATOR_POINT;
}

ElevatorPoint::~ElevatorPoint()
{
}

void ElevatorPoint::Update(float gameTime)
{
}

void ElevatorPoint::Draw()
{
}

int ElevatorPoint::IdRef() const
{
	return idRef;
}

float ElevatorPoint::Height() const
{
	return rect->Height();
}

float ElevatorPoint::Width() const
{
	return rect->Width();
}

Vector2 ElevatorPoint::Direction() const
{
	return direction;
}

int ElevatorPoint::DirectionOption() const
{
	return directionOp;
}

void ElevatorPoint::Height(float value)
{
	rect->Height(value);
}

void ElevatorPoint::Width(float value)
{
	rect->Width(value);
}

void ElevatorPoint::Direction(int value)
{
	directionOp = value;

	switch (value)
	{
		// (0 = UP, 1 = LEFT, 2 = RIGHT, 3 = DOWN, 4 = STOP)
		case 0:
			this->direction = moveUp;
			break;

		case 1:
			this->direction = moveLeft;
			break;

		case 2:
			this->direction = moveRight;
			break;

		case 3:
			this->direction = moveDown;
			break;

		case 4:
			this->direction = Vector2(0, 0);
			break;
	}
}
