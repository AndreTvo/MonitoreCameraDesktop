#include "Keyboard.h"

Keyboard::Keyboard(FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * inputQueue)
{
	_inputQueue = inputQueue;
	initialized = false;

	fstream fin;
	fin.open("keycodes.txt");

	if (fin.is_open())
	{
		// 27 ESC 111
		int windowsKeyCode = 0;
		string nameKey;
		int androidKeyCode = 0;

		while (!fin.eof())
		{
			fin >> windowsKeyCode >> nameKey >> androidKeyCode;
			keyData[windowsKeyCode] = androidKeyCode;
		}

		fin.close();

		initialized = true;
	}
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update(float gameTime)
{
	if (initialized)
	{
		KeyboardData keyCode;

		if ((keyCode = Engine::WindowManager()->KeyboardMessage()).first > 0)
		{
			KeyInput * keyInput = new KeyInput();
			keyInput->setKeyCode(keyData[keyCode.first]);
			keyInput->setKeyState(keyCode.second ? KeyInputState::KEY_DOWN : KeyInputState::KEY_UP);

			_inputQueue->Push(keyInput);
		}
	}
}

void Keyboard::Draw()
{
}
