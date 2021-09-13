#include "Component.h"
#include "Object.h"

void Component::SetOwner(Object * pOwner)
{
	this->pOwner = pOwner;
}

void Component::Enabled(bool value)
{
	enabled = value;
}

bool Component::Enabled() const
{
	return enabled;
}

Component::~Component()
{
}

void Component::PostInit()
{
}

void Component::FixedUpdate()
{
}

void Component::OnCollision(Object * obj)
{
}

void Component::OnCollisionEnter(Object * obj)
{
}

void Component::OnCollisionStay(Object * obj)
{
}

void Component::OnCollisionExit(Object * obj)
{
}
