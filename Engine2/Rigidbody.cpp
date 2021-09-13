#include "Rigidbody.h"
#include "PlatformPlayer.h"
#include "Object.h"

Rigidbody::~Rigidbody()
{
}

bool Rigidbody::Init()
{
	physicsFrameTime = Engine::PhysicsFrameTime();

	maxSpeed = 4;
	enabled = true;
	parent = nullptr;
	gravity = Vector2(0, 800);
	velocity = Vector2();

	return true;
}

void Rigidbody::PostInit()
{
}

void Rigidbody::FixedUpdate()
{
	acceleration += gravity * physicsFrameTime;
	velocity = acceleration * physicsFrameTime;

	if (abs(velocity.X()) > maxSpeed)
	{
		if (velocity.X() > maxSpeed)
		{
			acceleration.X(maxSpeed / physicsFrameTime);
			velocity.X(maxSpeed);
		}
		else
		{
			acceleration.X(-maxSpeed / physicsFrameTime);
			velocity.X(-maxSpeed);
		}
	}

	pOwner->Translate(velocity);

	if (parent)
		parent->Translate(velocity);
}

void Rigidbody::Update(float gameTime)
{
	if (!platform)
		platform = pOwner->Components()->GetComponent<PlatformPlayer>();
}

void Rigidbody::Draw()
{
}

Object * Rigidbody::Parent() const
{
	return parent;
}

void Rigidbody::Parent(Object * obj)
{
	parent = obj;
}

void Rigidbody::Gravity(Vector2 vect)
{
	gravity = vect;
}

Vector2 Rigidbody::Gravity() const
{
	return gravity;
}

Vector2 Rigidbody::Velocity() const
{
	return velocity;
}

void Rigidbody::AddForce(const Vector2 & vect)
{
	acceleration += vect;

	if (platform)
	{
		if (platform->IsGrounded() && !vect.X())
		{
			pOwner->Translate(vect.Unit());
		}
	}
}

void Rigidbody::Bounciness(float value)
{
	lossEnergy = value;
}

float Rigidbody::Bounciness() const
{
	return lossEnergy;
}

void Rigidbody::MaxSpeedX(float speed)
{
	maxSpeed = speed;
}

void Rigidbody::ResetAcceleration()
{
	acceleration.Clear();
}

void Rigidbody::ResetVelocity()
{
	velocity.Clear();
}

void Rigidbody::ResetXVelocity()
{
	velocity.X(0);
}

void Rigidbody::ResetYVelocity()
{
	velocity.Y(0);
}

void Rigidbody::ResetXAcceleration()
{
	acceleration.X(0);
}

void Rigidbody::ResetYAcceleration()
{
	acceleration.Y(0);
}
