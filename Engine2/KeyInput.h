#pragma once

#include "AndroidInputObject.h"

enum KeyInputState
{
	KEY_DOWN,
	KEY_UP
};

class KeyInput : public AndroidInputObject
{
private:
	int keyCode;
	KeyInputState keyState;

public:
	KeyInput();
	~KeyInput();

	void setKeyCode(int keyCode);
	void setKeyState(KeyInputState keyState);

	int getKeyCode() const;
	KeyInputState getKeyState() const;
	const wchar_t * getSerialization();

};