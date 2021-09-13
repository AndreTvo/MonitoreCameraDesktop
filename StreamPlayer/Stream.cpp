#include "stream.h"
#include <stdexcept>

#include <mutex>
#include <thread>

#include "frame.h"

using namespace std;
using namespace FFmpeg;
using namespace FFmpeg::Facade;

Stream::Stream(string const& streamUrl, int32_t connectionTimeoutInMilliseconds,
	ID3D11DeviceContext * gpuContext_,
	ID3D11Resource * gpuTexture_)
    : gpuContext_(gpuContext_), gpuTexture_(gpuTexture_), connectionTimeout_(connectionTimeoutInMilliseconds), stopRequested_(false), formatCtxPtr_(nullptr), codecCtxPtr_(nullptr),
    videoStreamIndex_(-1), imageConvertCtxPtr_(nullptr), completed_(false)
{
    workerThread_ = thread(&Stream::OpenAndRead, this, streamUrl);
	avframePtr = nullptr;
}

int Stream::InterruptCallback(void *ctx)
{
    Stream* streamPtr = reinterpret_cast<Stream*>(ctx);
    
    if (streamPtr == nullptr)
    {
        return 0;
    }

    {
        lock_guard<mutex> lock(streamPtr->mutex_);
        if (streamPtr->completed_)
        {
            return 0;
        }
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - streamPtr->connectionStart_);
    
    if (elapsed > streamPtr->connectionTimeout_)
    {
        return 1;
    }

    return 0;
}

void Stream::Open(string const& streamUrl)
{
    static once_flag flag;
    call_once(flag, []()
    {
        av_register_all();
        avdevice_register_all();
        avcodec_register_all();
        avformat_network_init();
    });

	Sleep(1000);

    formatCtxPtr_ = avformat_alloc_context();

    formatCtxPtr_->interrupt_callback.callback = InterruptCallback;
    formatCtxPtr_->interrupt_callback.opaque = this;
    formatCtxPtr_->flags |= AVFMT_FLAG_NONBLOCK;

    connectionStart_ = std::chrono::system_clock::now();
    
    int error = avformat_open_input(&formatCtxPtr_, streamUrl.c_str(), nullptr, nullptr);
    if (error != 0)
    {
        throw runtime_error("avformat_open_input() failed: " + AvStrError(error));
    }

    error = avformat_find_stream_info(formatCtxPtr_, nullptr);
    if (error < 0)
    {
        avformat_close_input(&formatCtxPtr_);
        throw runtime_error("avformat_find_stream_info() failed: " + AvStrError(error));
    }

    for (uint32_t i = 0; i < formatCtxPtr_->nb_streams; i++)
    {
        if (formatCtxPtr_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex_ = i;
            break;
        }
    }

    if (videoStreamIndex_ == -1)
    {
        avformat_close_input(&formatCtxPtr_);
        throw runtime_error("no video stream");
    }

    AVCodec *codecPtr = avcodec_find_decoder(formatCtxPtr_->streams[videoStreamIndex_]->codecpar->codec_id);
    if (codecPtr == nullptr)
    {
        avformat_close_input(&formatCtxPtr_);
        throw runtime_error("avcodec_find_decoder() failed");
    }

	codecCtxPtr_ = avcodec_alloc_context3(codecPtr);
	if (!codecCtxPtr_)
	{
		throw runtime_error("avcodec_alloc_context3() failed");
	}

	codecCtxPtr_->flags |= CODEC_FLAG_LOW_DELAY;
	codecCtxPtr_->flags2 |= CODEC_FLAG2_FAST;

    error = avcodec_open2(codecCtxPtr_, codecPtr, nullptr);
    if (error < 0)
    {
        avcodec_close(codecCtxPtr_);
        avformat_close_input(&formatCtxPtr_);
        throw runtime_error("avcodec_open2() failed: " + AvStrError(error));
    }
}

void Stream::Read()
{
	while (!stopRequested_)
	{
		AVPacket *packetPtr = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));
		av_init_packet(packetPtr);
		int error = av_read_frame(formatCtxPtr_, packetPtr);

		if (error < 0)
		{
			//if (error != static_cast<int>(AVERROR_EOF))
			//{
			//    throw runtime_error("av_read_frame() failed: " + AvStrError(error));
			//}

			stopRequested_ = true;

			// The end of a stream.
			break;
		}

		if (packetPtr->stream_index == videoStreamIndex_)
		{
			AVPacket* localPacketPtr = packetPtr;
			error = 0;

			avframePtr = av_frame_alloc();

			avcodec_send_packet(codecCtxPtr_, localPacketPtr);
			error = avcodec_receive_frame(codecCtxPtr_, avframePtr);

			if (error == 0)
			{
				frameQueue_.PushFrame(avframePtr);
			}
			else
			{
				av_frame_free(&avframePtr);
			}

			av_packet_unref(localPacketPtr);
		}
		else
		{
			av_packet_unref(packetPtr);
		}
	}

	if (stopRequested_)
	{
		if (workerFrameThread_.joinable())
		{
			workerFrameThread_.join();
		}
	}
}

void Stream::ProcessFramePackets()
{
	AVPacket * packetPtr = nullptr;
	int error = 0;

	while (!stopRequested_)
	{
		packetQueue_.WaitAndPop(packetPtr);

		avframePtr = av_frame_alloc();

		avcodec_send_packet(codecCtxPtr_, packetPtr);
		error = avcodec_receive_frame(codecCtxPtr_, avframePtr);

		if (error == 0)
		{
			frameQueue_.PushFrame(avframePtr);
		}
		else
		{
			av_frame_free(&avframePtr);
		}

		av_packet_unref(packetPtr);
	}
}

void Stream::OpenAndRead(string const& streamUrl)
{
    try
    {
        Open(streamUrl);
    }
    catch (runtime_error &e)
    {
        error_ = e.what();
    }

    {
        lock_guard<mutex> lock(mutex_);
        completed_ = true;
    }

    streamOpened_.notify_one();

    if (formatCtxPtr_ == nullptr || codecCtxPtr_ == nullptr || videoStreamIndex_ == -1)
    {
        return;
    }

    Read();
}

bool Stream::UpdateTexture()
{
	bool ret = false;

	if (ret = frameQueue_.TryPop(framePtr_))
	{
		int height = framePtr_->height;
		int rowPitch = framePtr_->width;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		gpuContext_->Map(gpuTexture_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		uint8_t* mappedData = static_cast<uint8_t*>(mappedResource.pData);

		int j, k;

		uint32_t pitchY = framePtr_->linesize[0];
		uint32_t pitchU = framePtr_->linesize[1];

		uint8_t *avY = framePtr_->data[0];
		uint8_t *avU = framePtr_->data[1];
		uint8_t *avV = framePtr_->data[2];

		int skipLineArea = 0;
		int halfPitch = rowPitch / 2;
		int skipLineValue = pitchU - halfPitch;
		int gpuPitchDiff = mappedResource.RowPitch - rowPitch;
		int uvCount = (height / 2) * halfPitch;

		for (j = 0; j < height; j++)
		{
			memcpy(mappedData, avY, rowPitch);
			mappedData += mappedResource.RowPitch;
			avY += pitchY;
		}

		// Copying 2 bytes at once is a little bit faster than 1 byte copy
		// But up to 4 bytes is slower
		// Dont change
		for (j = 0, k = 0; j < uvCount / 2; j++, k += 2, skipLineArea += 2)
		{
			if (skipLineArea == halfPitch)
			{
				k += skipLineValue;
				skipLineArea = 0;
				mappedData += gpuPitchDiff;
			}

			mappedData[0] = avU[k];
			mappedData[2] = avU[k + 1];
			mappedData++;

			mappedData[0] = avV[k];
			mappedData[2] = avV[k + 1];
			mappedData += 3;
		}

		gpuContext_->Unmap(gpuTexture_, 0);

		av_frame_free(&framePtr_);
	}

	return ret;
}

int32_t Stream::InterframeDelayInMilliseconds() const
{
    return codecCtxPtr_->ticks_per_frame * 1000 *
        codecCtxPtr_->time_base.num / codecCtxPtr_->time_base.den;
}

string Stream::AvStrError(int errnum)
{
    char buf[128];
    av_strerror(errnum, buf, sizeof(buf));
    return string(buf);
}

void Stream::Stop()
{
    stopRequested_ = true;
	packetQueue_.StopWait();
    frameQueue_.StopWait();

	if (workerThread_.joinable())
	{
		workerThread_.join();
	}

	AVPacket *packetPtr = nullptr;
	while (packetQueue_.TryPop(packetPtr))
	{
		av_packet_unref(packetPtr);
	}

    AVFrame *framePtr = nullptr;
    while (frameQueue_.TryPop(framePtr))
    {
        av_frame_free(&framePtr);
    }
}

Stream::~Stream()
{
    if (imageConvertCtxPtr_)
    {
        sws_freeContext(imageConvertCtxPtr_);
    }

    if (codecCtxPtr_)
    {
        avcodec_close(codecCtxPtr_);
    }

    if (formatCtxPtr_)
    {
        avformat_close_input(&formatCtxPtr_);
        avformat_free_context(formatCtxPtr_);
    }
}