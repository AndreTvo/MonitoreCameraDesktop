#include "Collision.h"
#include "Rigidbody.h"
#include "Position.h"
#include "Scene.h"
#include "Object.h"

Collision::~Collision()
{
	for (auto it : collisorTable)
	{
		it->Components()->GetComponent<Collision>()->CollisorTable()->erase(pOwner);
		it->Components()->GetComponent<Collision>()->UpdateCollisionFlag();
	}

	Engine::CurrentGame()->GetScene()->RemoveCollisionResolution(pOwner);

	if (bbox)
		delete bbox;
}

bool Collision::Init()
{
	bbox = nullptr;
	position = nullptr;
	trigger = false;
	rigidbody = nullptr;
	debug = true;
	enabled = true;

	color = ((255 & 0xff) << 24) + ((255 & 0xff) << 16) + ((0 & 0xff) << 8)
		+ (255 & 0xff);

	return true;
}

void Collision::PostInit()
{
	position = pOwner->Components()->GetComponent<Position>();
	vector = position->Vector();

	rigidbody = pOwner->Components()->GetComponent<Rigidbody>();

	Engine::CurrentGame()->GetScene()->AddCollisionResolution(pOwner);
}

void Collision::Update(float gameTime)
{
	if (!rigidbody)
		rigidbody = pOwner->Components()->GetComponent<Rigidbody>();
}

void Collision::Draw()
{
}

void Collision::IsColliding(bool value)
{
	colliding = value;
}

bool Collision::IsColliding() const
{
	return colliding;
}

void Collision::BoundingBox(Geometry * geometry)
{
	bbox = geometry;
}

Geometry * Collision::BoundingBox()
{
	return bbox;
}

void Collision::TriggerMode(bool value)
{
	trigger = value;
}

bool Collision::TriggerMode() const
{
	return trigger;
}

void Collision::Debug(bool value)
{
	debug = value;
}

bool Collision::Debug() const
{
	return debug;
}

void Collision::DebugCollision()
{
	color = ((255 & 0xff) << 24) + ((0 & 0xff) << 16) + ((100 & 0xff) << 8)
		+ (0 & 0xff);
}

Object * Collision::Parent()
{
	return parent;
}

void Collision::Parent(Object * obj)
{
	parent = obj;
}

uint Collision::Color() const
{
	return color;
}

HashCollision * Collision::CollisorTable()
{
	return &collisorTable;
}

void Collision::UpdateCollisionFlag()
{
	colliding = !collisorTable.empty();

	if (!colliding)
	{
		color = ((255 & 0xff) << 24) + ((255 & 0xff) << 16) + ((0 & 0xff) << 8)
			+ (255 & 0xff);
	}
}

Rigidbody * Collision::Body()
{
	return rigidbody;
}

Vector2 * Collision::Vector()
{
	return vector;
}
