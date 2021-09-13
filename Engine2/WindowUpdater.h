#pragma once

#include "Object.h"
#include <mutex>
#include "Engine.h"

using std::mutex;
using std::unique_lock;

class WindowUpdater : public Object
{
private:
	bool _updateWindow;
	int _orientation;

	mutex _windowUpdaterMutex;

public:
	WindowUpdater();
	~WindowUpdater();

	void UpdateWindow(int orientation);

	void Update(float gameTime);
	void Draw();
};