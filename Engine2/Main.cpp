/**********************************************************************************
// GeometryWars (Código Fonte)
// 
// Criação:		23 Out 2012
// Atualização:	25 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Jogo demonstrando Scrolling e IA
//
**********************************************************************************/

#include "Main.h"			// classe do jogo
#include "Engine.h"			// motor de jogo
#include <sstream>

#include <fstream>
using std::ifstream;

void Main::Configure(string ipaddress, string port, uint width, uint height, uint bitrate, string encoder, int phoneWidth, int phoneHeight)
{
	_phoneWidth = phoneWidth;
	_phoneHeight = phoneHeight;
	_ipaddress = ipaddress;
	_port = port;
	_width = width;
	_height = height;
	_bitrate = bitrate;
	_encoder = encoder;
}

void Main::Init()
{
	_fps = 60;
	_position = 0;
	_framePrepared = false;

	//updater = new WindowUpdater();
	maintainer = new NetworkMaintener();

	//mouse = new Mouse(maintainer->getQueue(), _phoneWidth - _width, _phoneHeight - _height);
	//keyboard = new Keyboard(maintainer->getQueue());
	
	/*
	stringstream ss;
	ss.str("");

	ss << "rtsp://" << _ipaddress << ":" << _port << "/?";
	ss << "resolutions=" << _width << "-" << _height << "-" << _bitrate << "-" << _fps << "-" << _encoder;*/

	viewport.left = 0;
	viewport.right = (Engine::WindowManager()->InternalWidth());

	viewport.top = 0;
	viewport.bottom = (Engine::WindowManager()->InternalHeight());

	scene->Add(maintainer, STATIC);
	//scene->Add(mouse, STATIC);
	//scene->Add(keyboard, STATIC);
	//scene->Add(updater, STATIC);
}

// ------------------------------------------------------------------------------

void Main::Update(float gameTime)
{
	scene->Update(gameTime);
	Engine::WindowManager()->CloseOnEscape();
}

// ------------------------------------------------------------------------------

void Main::Draw()
{
	scene->Draw();
	Background::ResetCounter();
}

// ------------------------------------------------------------------------------

void Main::Finalize()
{
	delete maintainer;
	delete scene;
}


// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

void f1(float ** matriz)
{
	stringstream ss;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			ss.str("");
			ss << matriz[i][j] << "\n";
			OutputDebugString(ss.str().c_str());
		}
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// cria motor do jogo
	Engine * engine = new Engine();

	ifstream fin;
	fin.open("config.ini");

	string type;
	string garbage;
	string ipaddress, port, encoder, low_latency_bool;
	uint width = 360, height = 640, bitrate = 1500, encoder_fps = 60, fps = 60;
	uint window_width = 360, window_height = 640;
	int phoneWidth = 1920, phoneHeight = 1080;

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> type;

			if (fin.good())
			{
				switch (str2int(type.c_str()))
				{
					case str2int("ipaddress"):
						fin >> ipaddress;
						break;

					case str2int("port"):
						fin >> port;
						break;

					case str2int("window_width"):
						fin >> window_width;
						break;

					case str2int("window_height"):
						fin >> window_height;
						break;

					case str2int("video_width"):
						fin >> width;
						break;

					case str2int("video_height"):
						fin >> height;
						break;

					case str2int("bitrate"):
						fin >> bitrate;
						break;

					case str2int("encoder_fps"):
						fin >> encoder_fps;
						break;

					case str2int("fps"):
						fin >> fps;
						break;

					case str2int("encoder"):
						fin >> encoder;
						break;

					case str2int("phone_width"):
						fin >> phoneWidth;
						break;

					case str2int("phone_height"):
						fin >> phoneHeight;
						break;

					default:
						fin >> garbage;
				}
			}
		}
	}

	fin.close();

	// configura janela
	engine->WindowManager()->Mode(WINDOWED);
	engine->WindowManager()->Size(window_width, window_height);
	engine->WindowManager()->InternalResolution(window_width, window_height);
	engine->WindowManager()->Color(0, 0, 0);
	engine->WindowManager()->Title("MonitoreLive");
	engine->WindowManager()->Icon(IDI_ICON);
	// engine->WindowManager()->Cursor(IDC_CURSOR);
	engine->WindowManager()->HideCursor(false);

	// configura dispositivo gráfico
	engine->GraphicsManager()->VSync(true);

	// cria e inicia o jogo
	Game * game = new Main();
	static_cast<Main*>(game)->Configure(ipaddress, port, width, height, bitrate, encoder, phoneWidth, phoneHeight);

	engine->Start(fps, game);

	delete engine;
	return 0;
}

// ----------------------------------------------------------------------------