#pragma once

#include "Object.h"

class Sphere : public Object
{
private:

public:
	Sphere();
	~Sphere();

	void Update(float gameTime);
	void Draw();
};