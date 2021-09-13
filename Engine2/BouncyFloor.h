#pragma once

#include "Object.h"

class BouncyFloor : public Object
{
private:
	Collision * collision;

public:
	BouncyFloor(float height, float width, float posX, float posY);
	~BouncyFloor();

	void Update(float gameTime);
	void Draw();

};