#include "streamplayer.h"
#include <string>
#include <sstream>
#include <cassert>

using namespace std;
using namespace FFmpeg;
using namespace FFmpeg::Facade;

StreamPlayer::StreamPlayer()
	: stopRequested_(false)
{
}

FFmpeg::Facade::StreamPlayer::~StreamPlayer()
{
	Stop();
}

void StreamPlayer::StartPlay(string const& streamUrl,
    uint32_t connectionTimeoutInMilliseconds,
	ID3D11DeviceContext * gpuContext,
	ID3D11Resource * gpuTexture)
{
	{
		unique_lock<mutex> lock(streamMutex_);
		streamPtr_ = make_unique<Stream>(streamUrl, connectionTimeoutInMilliseconds, gpuContext, gpuTexture);
	}
}

void StreamPlayer::Stop()
{
    stopRequested_ = true;

    {
        unique_lock<mutex> lock(streamMutex_);
        if (streamPtr_)
        {
            streamPtr_->Stop();
			streamPtr_.release();
        }
    }
}

bool StreamPlayer::UpdateTexture()
{
	if (streamPtr_)
		return streamPtr_->UpdateTexture();
	else
		return false;
}

