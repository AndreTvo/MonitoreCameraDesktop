#pragma once

#include "Object.h"
#include "KeyInput.h"
#include <fstream>
#include <ConcurrentQueue.h>

using std::ifstream;

class Keyboard : public Object
{
private:
	FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * _inputQueue;
	unordered_map<int, int> keyData;
	bool initialized;

public:
	Keyboard(FFmpeg::Facade::ConcurrentQueue<AndroidInputObject *> * inputQueue);
	~Keyboard();

	void Update(float gameTime);
	void Draw();

};
