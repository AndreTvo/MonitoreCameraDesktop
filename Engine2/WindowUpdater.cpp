#include "WindowUpdater.h"

WindowUpdater::WindowUpdater()
{
	_updateWindow = false;
	_orientation = 1;
}

WindowUpdater::~WindowUpdater()
{
}

void WindowUpdater::UpdateWindow(int orientation)
{
	unique_lock<mutex> lock(_windowUpdaterMutex);
	_orientation = orientation;
	_updateWindow = true;
}

void WindowUpdater::Update(float gameTime)
{
	unique_lock<mutex> lock(_windowUpdaterMutex);

	if (_updateWindow)
	{
		if (_orientation == 2)
		{
			Engine::WindowManager()->Resize(640, 360);
			Engine::GraphicsManager()->Resize(640, 360);
		}
		else if (_orientation == 1)
		{
			Engine::WindowManager()->Resize(360, 640);
			Engine::GraphicsManager()->Resize(360, 640);
		}

		_updateWindow = false;
	}
}

void WindowUpdater::Draw()
{
}
