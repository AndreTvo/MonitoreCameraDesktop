#pragma once

#include "Object.h"

enum MissileId
{
	PLAYER_SHOT
};

class Missile : public Object
{
private:
	float movX, movY;
	bool death;

	Image * image;
	Sprite * sprite;
	Collision * collision;

public:
	Missile(uint id, float movX, float movY);
	~Missile();

	void OnCollisionEnter(Object * obj);

	void Update(float gameTime);
	void Draw();

};