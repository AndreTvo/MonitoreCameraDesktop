#include "Sphere.h"

#define MOVE_SPHERE FALSE

Sphere::Sphere()
{
	MoveTo(450, 0);

	auto collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(new Circle(20));

	Components()->AddComponent<Rigidbody>();
}

Sphere::~Sphere()
{
}

void Sphere::Update(float gameTime)
{
#if MOVE_SPHERE
	if (Engine::InputManager()->KeyDown('D'))
	{
		Vector2 vec = Vector2(100, 0);
		position->Translate(vec * gameTime);
	}

	if (Engine::InputManager()->KeyDown('A'))
	{
		Vector2 vec = Vector2(-100, 0);
		position->Translate(vec * gameTime);
	}
#endif
}

void Sphere::Draw()
{
}
