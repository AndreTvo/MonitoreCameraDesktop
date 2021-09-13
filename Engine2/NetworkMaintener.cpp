#include "NetworkMaintener.h"
#include <sstream>

using namespace std;

#define DEFAULT_BUFLEN 1024

void NetworkMaintener::AnnouncerThread()
{
	UDPSocket socketAnnouncerReceiver;
	socketAnnouncerReceiver.Bind(5010);

	char monitoreLivePacket[1024] = { 0 };
	char ip[1024] = { 0 };

	stringstream ss;

	while (true)
	{
		sockaddr_in clientSockInfo = socketAnnouncerReceiver.RecvFrom(monitoreLivePacket, 1024);

		inet_ntop(AF_INET, &clientSockInfo.sin_addr, ip, 1024);

		ss << ip;
		socketAnnouncerReceiver.SendTo(clientSockInfo, ss.str().c_str(), ss.str().length());

		Sleep(2000);
		cout << "Recebido pacote de anuncio enviado pelo celular" << endl;

		ss.str("");
		ss << "rtsp://" << ip << ":1234/?";
		ss << "resolutions=1280-720-2000-24";

		_playerList.Push(new Background(ss.str()));

		memset(ip, 0, 1024);
		ss.str("");
	}
}

NetworkMaintener::NetworkMaintener()
{
	_wsaSession = new WSASession();	
	_announcerServer = thread(&NetworkMaintener::AnnouncerThread, this);
}

NetworkMaintener::~NetworkMaintener()
{
	delete _wsaSession;
}

void NetworkMaintener::Update(float gameTime)
{
	Background* bgPointer;

	while (_playerList.TryPop(bgPointer))
	{
		Engine::CurrentGame()->GetScene()->Add(bgPointer, STATIC);
	}
}

void NetworkMaintener::Draw()
{
}
