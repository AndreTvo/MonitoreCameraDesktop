#pragma once

#include "Object.h"
#include "TileSet.h"

#include "TouchInput.h"
#include <ConcurrentQueue.h>

class Main;

class Mouse : public Object
{
private:
	FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * _inputQueue;	
	bool debug;

	int _phoneWidthDiff;
	int _phoneHeightDiff;
	bool mouseLeftDownCtrl;
	bool mouseLeftUpCtrl;

public:
	Mouse(FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * inputQueue, int phoneWidth, int phoneHeight);
	~Mouse();

	void Update(float gameTime);
	void Draw();

	bool Click() const;
	bool RightClick() const;
	bool LeftClick() const;

	void Clean();
};