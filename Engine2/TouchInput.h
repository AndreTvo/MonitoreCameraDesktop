#pragma once

#include "AndroidInputObject.h"

enum TouchType
{
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_MOVING,
	MOUSE_WHEEL = 8
};

enum TouchWheelDirection
{
	UPWARDS = 1,
	DOWNWARDS = -1
};

class TouchInput : public AndroidInputObject
{
private:
	int x;
	int y;
	TouchType mouseType;
	TouchWheelDirection mouseWheelDirection;

public:
	TouchInput();
	~TouchInput();

	void setX(int x);
	void setY(int y);
	void setMouseType(TouchType mouseType);
	void setMouseWheelDirection(TouchWheelDirection mouseWheelDirection);

	int getX() const;
	int getY() const;
	TouchType getMouseType() const;
	TouchWheelDirection getMouseWheelDirection() const;
	const wchar_t * getSerialization();

};
