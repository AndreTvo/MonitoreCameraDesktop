#ifndef FFMPEG_FACADE_STREAM_H
#define FFMPEG_FACADE_STREAM_H

#include <cstdint>
#include <string>
#include <memory>
#include <chrono>

#include <d3d11.h>
#include <atomic>
#include <thread>
#include <mutex>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#pragma warning( push )
#pragma warning( disable : 4100 )


#pragma warning( pop )


#include "ConcurrentQueue.h"

#ifdef STREAMPLAYER_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

namespace FFmpeg
{

#pragma warning( push )
#pragma warning( disable : 4244 )

	extern "C"
	{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
	}

#pragma warning( pop )

	// The FFmpeg framework built using the following configuration:
	// x64: ./configure --toolchain=msvc --arch=amd64 --target-os=win64 --enable-version3 --enable-static --disable-shared --disable-programs --disable-doc
    // x86: ./configure --toolchain=msvc --arch=i386 --enable-version3 --enable-static --disable-shared --disable-programs --disable-doc 

#pragma comment(lib, "libavformat.a")
#pragma comment(lib, "libavcodec.a")
#pragma comment(lib, "libavdevice.a")
#pragma comment(lib, "libavfilter.a")
#pragma comment(lib, "libavutil.a")
#pragma comment(lib, "libswresample.a")
#pragma comment(lib, "libswscale.a")
// #pragma comment(lib, "libpostproc.a")
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Secur32.lib")

	namespace Facade
	{
		class Frame;

		/// <summary>
		/// A Stream class converts a stream into a set of frames. 
		/// </summary>
		class Stream
		{
		public:
			/// <summary>
			/// Initializes a new instance of the Stream class.
			/// </summary>
			/// <param name="streamUrl">The url of a stream to decode.</param>
            /// <param name="connectionTimeoutInMilliseconds">The connection timeout in milliseconds.</param>
            Stream(std::string const& streamUrl,
                int32_t connectionTimeoutInMilliseconds,
				ID3D11DeviceContext * gpuContext_,
				ID3D11Resource * gpuTexture_);

			/// <summary>
			/// Gets the next frame in the stream and draw to the DirectX Texture
			/// </summary>
            bool UpdateTexture();

			/// <summary>
			/// Gets an interframe delay, in milliseconds.
			/// </summary>
			int32_t InterframeDelayInMilliseconds() const;

            void Stop();

			/// <summary>
			/// Releases all resources used by the decoder.
			/// </summary>
			~Stream();

		private:

            void Open(std::string const& streamUrl);

            void Read();

			void ProcessFramePackets();

            void OpenAndRead(std::string const& streamUrl);

            static int InterruptCallback(void *ctx);

			static std::string AvStrError(int errnum);

            std::chrono::milliseconds connectionTimeout_;
            atomic<bool> stopRequested_;

			AVFormatContext *formatCtxPtr_;
			AVCodecContext  *codecCtxPtr_;			
			int32_t videoStreamIndex_;			
			SwsContext *imageConvertCtxPtr_;
			AVFrame *framePtr_;
			int frameFinished;
			int bytesUsed;

			// DirectX
			ID3D11DeviceContext * gpuContext_;
			ID3D11Resource * gpuTexture_;

			AVFrame *avframePtr;
            bool completed_;
            std::string error_;
            thread workerThread_;
			thread workerFrameThread_;
            mutex mutex_;
            condition_variable streamOpened_;

			ConcurrentQueue<AVPacket *> packetQueue_;
            ConcurrentQueue<AVFrame *> frameQueue_;

            std::chrono::time_point<std::chrono::system_clock> connectionStart_;            
		};
	}
}

#endif // FFMPEG_FACADE_STREAM_H