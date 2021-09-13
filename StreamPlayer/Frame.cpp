#include "frame.h"
#include <cassert>

using namespace std;
using namespace FFmpeg;
using namespace FFmpeg::Facade;

uint8_t * Frame::pixelsPtr_ = nullptr;

Frame::Frame(uint32_t width, uint32_t height, AVFrame &avFrame)
	: width_(width), height_(height)
{
	UINT rowPitch = ((width + 1) >> 1) * 2;
	UINT imageSize = (rowPitch * height) + ((rowPitch * height + 1) >> 1);

	if (!pixelsPtr_)
	{
		pixelsPtr_ = new uint8_t[imageSize];
	}

	int j, k, shift, position = 0;

	uint32_t pitchY = avFrame.linesize[0];
	uint32_t pitchU = avFrame.linesize[1];
	uint32_t pitchV = avFrame.linesize[2];

	uint8_t *avY = avFrame.data[0];
	uint8_t *avU = avFrame.data[1];
	uint8_t *avV = avFrame.data[2];

	for (j = 0; j < height; j++)
	{				
		::CopyMemory(pixelsPtr_ + position, avY, (width));
		position += (width);
		avY += pitchY;
	}

	int skipLineArea = 0;
	int uvCount = (height >> 1) * (width >> 1);

	for (j = 0, k = 0; j < uvCount; j++, k++)
	{
		if (skipLineArea == (width >> 1))
		{
			k += pitchU - (width >> 1);
			skipLineArea = 0;
		}

		pixelsPtr_[position++] = avU[k];
		pixelsPtr_[position++] = avV[k];
		skipLineArea++;
	}
}

void Frame::Draw(HWND window)
{   
}

void Frame::ToBmp(uint8_t **bmpPtr)
{
}