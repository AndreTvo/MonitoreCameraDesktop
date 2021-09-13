#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Network.h"
#include <string>
#include <ConcurrentQueue.h>
#include "Object.h"
#include "Background.h"
using std::string;

using namespace FFmpeg;
using namespace FFmpeg::Facade;

struct MonitoreLivePacket
{
	int type;
	int size;
	char* data;
};

class NetworkMaintener : public Object
{
private:
	ConcurrentQueue<Background*> _playerList;
	WSASession* _wsaSession;

	UINT_PTR _inputSocket;
	string _ipAddress;

	void AnnouncerThread();
	void ClientThread();

	thread _announcerServer;
	thread _clientManager;

	mutex _socketMutex;
	condition_variable _socketValid;
	bool _socketConnected;

public:
	NetworkMaintener();
	~NetworkMaintener();

	void Update(float gameTime);
	void Draw();

};
