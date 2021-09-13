#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <thread>

#pragma warning( push )
#pragma warning( disable : 4100 )


#pragma warning( pop )

#pragma warning( disable : 4251 )


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef STREAMPLAYER_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "stream.h"
#include "frame.h"

namespace FFmpeg
{
    namespace Facade
    {
        class Stream;

        /// <summary>
        /// A StreamPlayer class implements a stream playback functionality.
        /// </summary>
        class DLL StreamPlayer
        {
        public:

            /// <summary>
            /// Initializes a new instance of the StreamPlayer class.
            /// </summary>
            StreamPlayer();
			
			~StreamPlayer();

            /// <summary>
            /// Asynchronously plays a stream.
            /// </summary>
            /// <param name="streamUrl">The url of a stream to play.</param>
            /// <param name="connectionTimeoutInMilliseconds">The connection timeout in milliseconds.</param>
            void StartPlay(std::string const& streamUrl,
                uint32_t connectionTimeoutInMilliseconds,
				ID3D11DeviceContext * gpuContext,
				ID3D11Resource * gpuTexture);

            /// <summary>
            /// Stops a stream.
            /// </summary>
            void Stop();

			bool UpdateTexture();

        private:

			ID3D11Resource* _playerTexture;
            mutex streamMutex_;
            std::unique_ptr<Stream> streamPtr_;
			atomic<bool> stopRequested_;

            // There is a bug in the Visual Studio std::thread implementation,
            // which prohibits dll unloading, that is why the boost::thread is used instead.
            // https://connect.microsoft.com/VisualStudio/feedback/details/781665/stl-using-std-threading-objects-adds-extra-load-count-for-hosted-dll#tabs 

            mutex workerThreadMutex_;
			thread workerThread_;

            mutex errorMutex_;
            std::string error_;
        };
    }
}