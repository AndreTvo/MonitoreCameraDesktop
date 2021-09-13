/**********************************************************************************
// Engine (Arquivo de Cabe�alho)
//
// Cria��o:		15 Mai 2014
// Atualiza��o: 23 Mai 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	A fun��o da Engine � rodar jogos criados a partir da classe 
//              abstrata Game. Todo jogo deve ser uma classe derivada de Game 
//              e portanto deve implementar as fun��es membro Init, Update, Draw 
//              e Finalize, que ser�o chamadas pelo motor em um loop de tempo real. 
//              Para usar a classe Engine, o programador deve criar uma inst�ncia 
//              e chamar o m�todo Start(), passando um objeto derivado de Game.
//
**********************************************************************************/

#pragma once

// ---------------------------------------------------------------------------------


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Game.h"						// implementa��o do jogo
#include "Window.h"						// janela do jogo
#include "Graphics.h"					// hardware gr�fico
#include "Renderer.h"					// renderizador de sprites
#include "Timer.h"						// medidor de tempo

#include "Audio.h"						// implementa��o do audio
#include "Input.h"						// implementa��o do input

// ---------------------------------------------------------------------------------

class Engine
{
private:
	static Audio * audio;				// audio do jogo
	static Input * input;				// input do jogo

	static Game * game;					// jogo a ser executado
	static bool gamePaused;				// estado do jogo
	static Timer timer;					// medidor de tempo

	static Window   * window;			// janela do jogo
	static Graphics * graphics;			// dispositivo gr�fico
	static Renderer * renderer;			// renderizador de sprites

	static MSG msg;
	static float minFPS;
	static float averageFPS;
	static float maxFPS;
	static float stackedFPS;
	static int frameTimes;

	static double engineFPS;
	static const float physicsFrameTime;
	static int mouseWheelDirection;

	int Run();							// inicia execu��o do jogo

	static float FrameTime();			// calcula o tempo do quadro
	static DWORD WINAPI Engine::UpdateGame(LPVOID lpParam);

public:
	static Audio * AudioManager();
	static Input * InputManager();
	static Renderer * RenderManager();
	static Window * WindowManager();
	static Graphics * GraphicsManager();
	static Game * CurrentGame();

	static float GetMinFPS();
	static float GetAverageFPS();
	static float GetMaxFPS();

	static float PhysicsFrameTime();

	Engine();							// construtor
	~Engine();							// destrutor

	int Start(double engineEstimatedFPS, Game * level);			// inicia o game loop
	static void Pause();				// pausa o game loop
	static void Resume();				// reinicia o game loop
	
	template<class T> 
	static void Next()					// muda para pr�ximo n�vel do jogo
	{
		if (game)
		{
			game->Finalize();
			delete game;
			game = new T();
			game->Init();
		}
	};
};

// ---------------------------------------------------------------------------------