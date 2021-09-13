#include "Position.h"
#include "Collision.h"
#include "Object.h"

Position::~Position()
{
}

bool Position::Init()
{
	enabled = true;
	collision = nullptr;

	return true;
}

void Position::PostInit()
{
}

void Position::Update(float gameTime)
{
	if (!collision)
	{
		collision = pOwner->Components()->GetComponent<Collision>();

		if (collision)
			collision->BoundingBox()->MoveTo(vector.X(), vector.Y());
	}
}

void Position::Draw()
{
}

Vector2 * Position::Vector()
{
	return &vector;
}

float Position::X() const
{
	return vector.X();
}

float Position::Y() const
{
	return vector.Y();
}

float Position::Depth() const
{
	return vector.Depth();
}

void Position::X(float value)
{
	vector.X(value);
}

void Position::Y(float value)
{
	vector.Y(value);
}

void Position::Depth(float value)
{
	vector.Depth(value);
}

void Position::Translate(float dx, float dy)
{
	vector.Translate(dx, dy);

	if (collision)
		collision->BoundingBox()->Translate(dx, dy);
}

void Position::Translate(Vector2 vec)
{
	vector.Translate(vec);

	if (collision)
		collision->BoundingBox()->Translate(vec);
}

void Position::MoveTo(float px, float py)
{
	vector.MoveTo(px, py);

	if (collision)
		collision->BoundingBox()->MoveTo(px, py);
}

void Position::MoveTo(Vector2 vec)
{
	vector.MoveTo(vec);

	if (collision)
		collision->BoundingBox()->MoveTo(vec);
}
