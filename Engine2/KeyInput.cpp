#include "KeyInput.h"

KeyInput::KeyInput()
{
	type = KEY;
}

KeyInput::~KeyInput()
{
}

void KeyInput::setKeyCode(int keyCode)
{
	this->keyCode = keyCode;
}

void KeyInput::setKeyState(KeyInputState keyState)
{
	this->keyState = keyState;
}

int KeyInput::getKeyCode() const
{
	return keyCode;
}

KeyInputState KeyInput::getKeyState() const
{
	return keyState;
}

const wchar_t * KeyInput::getSerialization()
{
	dataStream.str("");

	WriteHeader();

	dataStream << "key_code=" << keyCode << EOL;
	dataStream << "key_state=" << keyState << EOL;

	WriteSignature();

	const size_t cSize = strlen(dataStream.str().c_str()) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(nullptr, wc, cSize, dataStream.str().c_str(), cSize);

	return wc;
}
