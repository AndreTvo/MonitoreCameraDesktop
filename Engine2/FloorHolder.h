#pragma once

#include "Object.h"

class FloorHolder : public Object
{
private:
	Rect * rect;
	Collision * collision;

public:
	FloorHolder();
	~FloorHolder();

	void Update(float gameTime);
	void Draw();

	float Width() const;
	void Width(float value);
};