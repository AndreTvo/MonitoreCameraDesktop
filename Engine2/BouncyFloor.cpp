#include "BouncyFloor.h"

BouncyFloor::BouncyFloor(float height, float width, float posX, float posY)
{
	position->MoveTo(posX, posY);

	collision = container->AddComponent<Collision>();
	collision->BoundingBox(new Rect(height, width));

	type = 2;
}

BouncyFloor::~BouncyFloor()
{
}

void BouncyFloor::Update(float gameTime)
{
}

void BouncyFloor::Draw()
{
}
