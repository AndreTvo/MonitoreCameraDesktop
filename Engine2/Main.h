/**********************************************************************************
// GeometryWars (Arquivo de Cabe�alho)
// 
// Cria��o:		23 Out 2012
// Atualiza��o:	25 Jul 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Jogo demonstrando Scrolling e IA
//
**********************************************************************************/

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define Rad2Deg 360 / (3.1416 * 2)

// ------------------------------------------------------------------------------

#include "Resources.h"				// recursos utilizados no jogo (cursor, �cone, etc.) 
#include "Game.h"					// defini��o da classe jogo
#include "Font.h"					// fonte para exibi��o de texto
#include "Sprite.h"					// defini��o dos sprites
#include "Scene.h"					// defini��o da cena do jogo
#include "Background.h"				// defini��o do pano de fundo
#include <sstream>					// sa�da para strings
#include <StreamPlayer.h>
#include "NetworkMaintener.h"
#include <ConcurrentQueue.h>
#include <list>

using namespace std;
using namespace FFmpeg::Facade;

// ------------------------------------------------------------------------------

class Main : public Game
{
private:	
	ID3D11Resource* mapTexture;

	NetworkMaintener * maintainer;

	Image * image;

	bool _framePrepared;
	int _position;

	uint _width;
	uint _height;
	int _phoneWidth;
	int _phoneHeight;
	uint _fps;

	string _ipaddress;
	string _port;

	string _encoder;

	uint _bitrate;

public:

	void Configure(string ipaddress, string port, uint width, uint height, uint bitrate, string encoder, int phoneWidth, int phoneHeight);

	void Init();					// inicializa jogo
	void Update(float gameTime);	// atualiza l�gica do jogo
	void Draw();					// desenha jogo
	void Finalize();				// finaliza jogo
};

// ---------------------------------------------------------------------------------