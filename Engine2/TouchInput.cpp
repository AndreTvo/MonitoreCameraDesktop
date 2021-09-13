#include "TouchInput.h"

TouchInput::TouchInput()
{
	type = TOUCH;

	x = 0;
	y = 0;

	mouseType = TouchType::BUTTON_DOWN;
	mouseWheelDirection = TouchWheelDirection::UPWARDS;
}

TouchInput::~TouchInput()
{
}

void TouchInput::setX(int x)
{
	this->x = x;
}

void TouchInput::setY(int y)
{
	this->y = y;
}

void TouchInput::setMouseType(TouchType mouseType)
{
	this->mouseType = mouseType;
}

void TouchInput::setMouseWheelDirection(TouchWheelDirection mouseWheelDirection)
{
	this->mouseWheelDirection = mouseWheelDirection;
}

int TouchInput::getX() const
{
	return x;
}

int TouchInput::getY() const
{
	return y;
}

TouchType TouchInput::getMouseType() const
{
	return mouseType;
}

TouchWheelDirection TouchInput::getMouseWheelDirection() const
{
	return mouseWheelDirection;
}

const wchar_t * TouchInput::getSerialization()
{
	dataStream.str("");

	WriteHeader();

	dataStream << "mouse_x=" << x << EOL;
	dataStream << "mouse_y=" << y << EOL;
	dataStream << "mouse_wheel_direction=" << mouseWheelDirection << EOL;
	dataStream << "mouse_type=" << mouseType << EOL;

	WriteSignature();

	const size_t cSize = strlen(dataStream.str().c_str()) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(nullptr, wc, cSize, dataStream.str().c_str(), cSize);

	return wc;
}
