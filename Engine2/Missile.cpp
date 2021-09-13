#include "Missile.h"

Missile::Missile(uint id, float movX, float movY) : movX(movX), movY(movY)
{
	sprite = Components()->AddComponent<Sprite>();
	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(new Circle(2));

	type = MISSILE;

	switch (id)
	{
		case PLAYER_SHOT:			
			sprite->LoadImageFile("Resources/Player/BM.png");
			break;

	}
}

Missile::~Missile()
{
}

void Missile::OnCollisionEnter(Object * obj)
{
	death = true;
}

void Missile::Update(float gameTime)
{
	if (X() > Engine::CurrentGame()->WorldWidth() || X() < 0)
		death = true;

	if (death)
		Engine::CurrentGame()->GetScene()->Remove();
	else
		Translate(movX * gameTime, movY * gameTime);
}

void Missile::Draw()
{
}

