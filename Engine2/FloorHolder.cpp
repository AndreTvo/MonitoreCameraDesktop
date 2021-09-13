#include "FloorHolder.h"

FloorHolder::FloorHolder()
{
	collision = container->AddComponent<Collision>();
	collision->BoundingBox(rect = new Rect(5, 1));
}

FloorHolder::~FloorHolder()
{
}

void FloorHolder::Update(float gameTime)
{
}

void FloorHolder::Draw()
{
}

float FloorHolder::Width() const
{
	return rect->Width();
}

void FloorHolder::Width(float value)
{
	rect->Width(value);
}
