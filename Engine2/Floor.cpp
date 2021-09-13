#include "Floor.h"

int Floor::refcount = 0;

Floor::Floor(float height, float width, float posX, float posY, bool passThrough) : passthrough(passThrough)
{
	idRef = refcount++;
	position->MoveTo(posX, posY);

	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(rect = new Rect(height, width));

	Components()->AddComponent<Rigidbody>()->Gravity(Vector2(0, 0));

	type = PLATFORM;
}

Floor::~Floor()
{
}

void Floor::Update(float gameTime)
{
}

void Floor::Draw()
{
}

void Floor::ResetIdRef()
{
	refcount = 0;
}

int Floor::IdRef() const
{
	return idRef;
}

float Floor::Height() const
{
	return rect->Height();
}

float Floor::Width() const
{
	return rect->Width();
}

void Floor::Height(float value)
{
	rect->Height(value);
}

void Floor::Width(float value)
{
	rect->Width(value);
}

void Floor::Passthrough(bool value)
{
	passthrough = value;
}

bool Floor::Passthrough() const
{
	return passthrough;
}

void Floor::Elevator(bool value)
{
	elevator = value;

	if (elevator)
	{
		fixedX = X();
		fixedY = Y();
	}
}

bool Floor::Elevator() const
{
	return elevator;
}

void Floor::FixedX(float value)
{
	fixedX = value;
}

void Floor::FixedY(float value)
{
	fixedY = value;
}

float Floor::FixedX() const
{
	return fixedX;
}

float Floor::FixedY() const
{
	return fixedY;
}
