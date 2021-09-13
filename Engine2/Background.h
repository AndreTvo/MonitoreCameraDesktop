/**********************************************************************************
// Background (Arquivo de Cabeçalho)
// 
// Criação:		07 Dez 2011
// Atualização:	30 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Define uma classe para construir backgrounds
//
**********************************************************************************/

#pragma once

// -------------------------------------------------------------------------------

#include "Types.h"          // tipos específicos da engine
#include "Image.h"			// imagem do background
#include <string>			// string de C++
#include "Object.h"
#include "Font.h"
using std::string;			// usando strings

#include <StreamPlayer.h>

// -------------------------------------------------------------------------------

class Background : public Object
{
private:
	static int backgroundCounter;

	string urlRTSP;
	Font* font;

	int retryCount;

	ID3D11Resource* texture;
	FFmpeg::Facade::StreamPlayer* streamPlayer;
	int timeOutCounter;

	SpriteInfo spriteInfo;				// informações do imageSprite 
	Image * image;				// imagem da superfície

	void StartConnection();

public:
	Background(string url);
	~Background();						// destrutor

	void Update(float gameTime);
	void Draw();	// desenha uma porção da imagem na tela

	uint Width();						// retorna a largura do background
    uint Height();						// retorna a altura do background
	
	static void ResetCounter();
}; 

// ---------------------------------------------------------------------------------
// Funções Inline

// retorna a largura do imageSprite
inline uint Background::Width() 
{ return image->Width(); }

// retorna a altura do imageSprite
inline uint Background::Height() 
{ return image->Height(); }

inline void Background::ResetCounter()
{
	backgroundCounter = 0;
}
