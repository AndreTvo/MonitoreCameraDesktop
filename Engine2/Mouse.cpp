#include "Mouse.h"
#include "Main.h"
#include <fstream>

using std::ofstream;

Mouse::Mouse(FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * inputQueue, int phoneWidth, int phoneHeight)
{
	_phoneWidthDiff = phoneWidth;
	_phoneHeightDiff = phoneHeight;

	_inputQueue = inputQueue;
	debug = true;
	type = MOUSE;

	mouseLeftUpCtrl = false;
	mouseLeftDownCtrl = true;
}

Mouse::~Mouse()
{
}

void Mouse::Update(float gameTime)
{
	if (debug)
	{
		float ajusteX = Engine::WindowManager()->Width() / (float)(Engine::WindowManager()->InternalWidth() + _phoneHeightDiff);
		float ajusteY = Engine::WindowManager()->Height() / (float)(Engine::WindowManager()->InternalHeight() + _phoneWidthDiff);

		// ajusta coordenadas para viewport
		float posX = ((Engine::WindowManager()->Mouse().x) / ajusteX) + Game::viewport.left;
		float posY = ((Engine::WindowManager()->Mouse().y) / ajusteY) + Game::viewport.top;
		
		if (LeftClick())
		{
			// Left Click Down
			if (!mouseLeftUpCtrl)
			{
				TouchInput * touchInput = new TouchInput();
				touchInput->setX(posX);
				touchInput->setY(posY);
				touchInput->setMouseType(TouchType::BUTTON_DOWN);

				_inputQueue->Push(touchInput);

				mouseLeftUpCtrl = true;
				mouseLeftDownCtrl = true;
			}
			else if (mouseLeftDownCtrl)
			{
				TouchInput * touchInput = new TouchInput();
				touchInput->setX(posX);
				touchInput->setY(posY);
				touchInput->setMouseType(TouchType::BUTTON_MOVING);

				_inputQueue->Push(touchInput);
			}
		}
		else
		{
			// Left Click Up

			if (mouseLeftUpCtrl)
			{
				mouseLeftUpCtrl = false;
				mouseLeftDownCtrl = false;

				TouchInput * touchInput = new TouchInput();
				touchInput->setX(posX);
				touchInput->setY(posY);
				touchInput->setMouseType(TouchType::BUTTON_UP);

				_inputQueue->Push(touchInput);
			}
			else
			{
				int wheel = Engine::WindowManager()->MouseWheel();

				if (wheel != 0)
				{
					Engine::WindowManager()->MouseWheel(0);

					TouchInput * touchInput = new TouchInput();
					touchInput->setX(posX);
					touchInput->setY(posY);
					touchInput->setMouseType(TouchType::MOUSE_WHEEL);
					touchInput->setMouseWheelDirection((TouchWheelDirection)wheel);

					_inputQueue->Push(touchInput);
				}
			}
		}
	}
}

void Mouse::Draw()
{
	if (debug)
	{
		
	}
}

bool Mouse::Click() const
{
	return LeftClick();
}

bool Mouse::LeftClick() const
{
	if (Engine::WindowManager()->MouseLeftButton())
		return true;

	return false;
}

void Mouse::Clean()
{
	
}


