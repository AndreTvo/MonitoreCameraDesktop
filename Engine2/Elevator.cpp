#include "Elevator.h"
#include "Object.h"
#include "Rigidbody.h"
#include "ElevatorPoint.h"
#include "Player.h"

Elevator::~Elevator()
{
}

bool Elevator::Init()
{
	bActivate = true;
	enabled = true;
	return true;
}

void Elevator::PostInit()
{
	rigidBody = pOwner->Components()->GetComponent<Rigidbody>();
	collision = pOwner->Components()->GetComponent<Collision>();
}

void Elevator::OnCollisionEnter(Object * obj)
{
	if (obj->Type() == ELEVATOR_POINT)
	{
		ElevatorPoint * ePoint = (ElevatorPoint *)obj;

		rigidBody->ResetVelocity();
		rigidBody->ResetAcceleration();

		rigidBody->AddForce(ePoint->Direction());
	}

	if (obj->Type() == PLAYER)
	{
		rigidBody->Parent(obj);
	}
}

void Elevator::OnCollisionStay(Object * obj)
{
	if (obj->Type() == PLAYER)
	{
		rigidBody->Parent(obj);
	}
}

void Elevator::OnCollisionExit(Object * obj)
{
	if (obj->Type() == PLAYER)
	{
		rigidBody->Parent(nullptr);
	}
}

bool Elevator::Autoplay() const
{
	return bActivate;
}

void Elevator::Autoplay(bool value)
{
	bActivate = value;
}

void Elevator::FixedUpdate()
{

}

void Elevator::Update(float gameTime)
{
	if (rigidBody)
	{
		
	}
	else
	{
		rigidBody = pOwner->Components()->GetComponent<Rigidbody>();
	}
}

void Elevator::Draw()
{
}
