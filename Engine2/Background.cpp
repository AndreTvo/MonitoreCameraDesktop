/**********************************************************************************
// Background (Código Fonte)
// 
// Criação:		07 Dez 2011
// Atualização:	30 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Define uma classe para construir backgrounds
//
**********************************************************************************/


#include "Background.h"
#include "Engine.h"

int Background::backgroundCounter = 0;

// -------------------------------------------------------------------------------

void Background::StartConnection()
{
}

Background::Background(string url)
{
	image = new Image(&texture, 1280, 720);
	font = new Font("Resources/NSimSun.png");
	font->Spacing("Resources/NSimSun.dat");
	urlRTSP = url;

	timeOutCounter = 0;
	retryCount = 0;

	// configura registro spriteInfo
	spriteInfo.x = 180;
	spriteInfo.y = Engine::WindowManager()->InternalHeight() / 2;
	spriteInfo.scale = 1.0f;
	spriteInfo.depth = 1.0f;
	spriteInfo.rotation = 0.0f;
	spriteInfo.color = Color(1, 1, 1, 1);
	spriteInfo.texture = image->View();
	spriteInfo.width = 360;
	spriteInfo.height = 640;

	streamPlayer = new FFmpeg::Facade::StreamPlayer();
	streamPlayer->StartPlay(urlRTSP, 5000, Engine::GraphicsManager()->Context(), texture);
}

// -------------------------------------------------------------------------------

Background::~Background()
{
	delete font;
	delete image;
	delete streamPlayer;
}

// -------------------------------------------------------------------------------

void Background::Update(float gameTime)
{
	if (!streamPlayer->UpdateTexture())
		timeOutCounter++;
	else
		timeOutCounter = 0;

	if (timeOutCounter > 300)
	{
		if (retryCount < 3)
		{
			timeOutCounter = 0;
			streamPlayer->Stop();
			streamPlayer->StartPlay(urlRTSP, 5000, Engine::GraphicsManager()->Context(), texture);
		}
		else
			Engine::CurrentGame()->GetScene()->Remove();

		retryCount++;
	}
}

void Background::Draw()
{
	spriteInfo.x = 180 + 360 * backgroundCounter;


	backgroundCounter++;

	// adiciona o imageSprite na lista de desenho
	Engine::RenderManager()->Draw(spriteInfo);

	font->Draw(spriteInfo.x, 10, urlRTSP);
}

// -------------------------------------------------------------------------------

