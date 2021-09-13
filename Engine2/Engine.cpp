/**********************************************************************************
// Engine (Código Fonte)
//
// Criação:		15 Mai 2014
// Atualização: 23 Mai 2014
// Compilador:	Visual C++ 12
//
// Descrição:	A função da Engine é rodar jogos criados a partir da classe
//              abstrata Game. Todo jogo deve ser uma classe derivada de Game
//              e portanto deve implementar as funções membro Init, Update, Draw
//              e Finalize, que serão chamadas pelo motor em um loop de tempo real.
//              Para usar a classe Engine, o programador deve criar uma instância
//              e chamar o método Start(), passando um objeto derivado de Game.
//
**********************************************************************************/

#include "Engine.h"
#include <timeapi.h>
#include <windows.h>
#include <sstream>
#include <chrono>
#include <thread>

using std::stringstream;
using namespace std::chrono;

// ------------------------------------------------------------------------------
// Inicialização de variáveis estáticas da classe

Game     * Engine::game     = nullptr;		// jogo em execução
Window   * Engine::window   = nullptr;		// janela do jogo
Graphics * Engine::graphics = nullptr;		// dispositivo gráfico
Renderer * Engine::renderer = nullptr;		// renderizador de sprites
Audio	 * Engine::audio	= nullptr;		// audio da engine
Input	 * Engine::input	= nullptr;		// input da engine
MSG  Engine::msg = { 0 };
float Engine::minFPS = INT_MAX;
float Engine::averageFPS = 0;
float Engine::maxFPS = 0;
float Engine::stackedFPS = 0;
double Engine::engineFPS = 0;
const float Engine::physicsFrameTime = 1.0f / 60;
int Engine::frameTimes = 0;

bool Engine::gamePaused = false;			// estado do game loop
Timer Engine::timer;						// medidor de tempo

// -------------------------------------------------------------------------------

Engine::Engine()
{
	gamePaused	= false;
	window		= new Window();
	graphics	= new Graphics();
	renderer	= new Renderer();
	audio		= new Audio();
	input		= new Input();
}

// -------------------------------------------------------------------------------

Engine::~Engine()
{
	if (game)
		delete game;

	delete renderer;
	delete graphics;
	delete window;
	delete audio;
	delete input;
}

// -----------------------------------------------------------------------------

void Engine::Pause()
{
	gamePaused = true;
	timer.Stop();
}

// -----------------------------------------------------------------------------

void Engine::Resume()
{
	gamePaused = false;
	timer.Start();
}

// -----------------------------------------------------------------------------

int Engine::Start(double engineEstimatedFPS, Game * gameLevel)
{
	game = gameLevel;
	engineFPS = engineEstimatedFPS;

	// cria janela do jogo
	window->Initialize();

	// inicializa dispositivo gráfico
	graphics->Initialize(window);

	// inicializa renderizador de sprites
	renderer->Initialize(window, graphics);

	// faz escalonamento do tamanho do backbuffer com o tamanho da janela
	graphics->Resize(window->Width(), window->Height());

	// ajusta a resolução do Sleep para 1 milisegundo
	// requer uso da biblioteca winmm.lib
	timeBeginPeriod(1);

	int exitCode = Run();

	// volta a resolução do Sleep ao valor original
	timeEndPeriod(1);

	return exitCode;
}

// -------------------------------------------------------------------------------

int Engine::Run()
{
	// inicialização do jogo
	game->Init();

	// mensagens do Windows
	msg = { 0 };

	// inicia contagem de tempo
	timer.Start();

	// cria uma thread exclusiva para a atualização do jogo
	HANDLE GameThread = CreateThread(NULL, 0, UpdateGame, NULL, 0, NULL);

	// loop principal do jogo
	do
	{
		// testa se tem mensagem do windows para tratar
		if (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	} while (msg.message != WM_QUIT);

	WaitForSingleObject(GameThread, INFINITE);
	CloseHandle(GameThread);

	// finalização do jogo
	game->Finalize();

	// encerra aplicação
	return int(msg.wParam);
}

DWORD WINAPI Engine::UpdateGame(LPVOID lpParam)
{
	std::chrono::high_resolution_clock::time_point a = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point b = std::chrono::high_resolution_clock::now();

	while (msg.message != WM_QUIT)
	{
		// -----------------------------------------------

		double wait_fps = (1/engineFPS)*1000;

		if (!gamePaused)
		{
			// atualiza informações de dispositivos de entrada
			input->Update();

			// atualização do jogo 
			game->Update(Timer::GameTime(FrameTime()));

			// limpa a tela para o próximo quadro
			graphics->Clear();

			// desenha o jogo
			game->Draw();

			// renderiza sprites
			renderer->Render();

			// apresenta o jogo na tela (troca backbuffer/frontbuffer)
			graphics->Present();
		}
		else
		{
			game->OnPause();
		}
	}

	// ----------------------------------------------
	return 0;
}

// -----------------------------------------------------------------------------

float Engine::FrameTime()
{
	static float frameTime = 0.0f;	// tempo transcorrido para o quadro atual

	// ----- START DEBUG ----------
//#ifdef _DEBUG
	static float totalTime = 0.0f;	// tempo total transcorrido 
	static uint  frameCount = 0;	// contador de frames transcorridos
//#endif
	// ------ END DEBUG -----------

	// tempo do frame atual
	frameTime = timer.Reset();

	// ----- START DEBUG ----------
//#ifdef _DEBUG
	// tempo acumulado dos frames
	totalTime += frameTime;

	// incrementa contador de frames
	frameCount++;

	// a cada 200ms atualiza indicador de FPS na janela
	if (totalTime >= 0.250f)
	{
		frameTimes++;
		int fps = frameCount / totalTime;
		stringstream text;			// fluxo de texto para mensagens
		text << std::fixed;			// sempre mostra a parte fracionária
		text.precision(3);			// três casas depois da vírgula

		text << window->Title().c_str() << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << frameTime * 1000 << " (ms)";

		SetWindowText(window->Id(), text.str().c_str());

		frameCount = 0;

		stackedFPS += fps;

		if (minFPS > fps)
			minFPS = fps;

		averageFPS = (stackedFPS) / frameTimes;

		if (maxFPS < fps)
			maxFPS = fps;

		totalTime = 0;
	}
//#endif
	// ------ END DEBUG ----------- 

	return frameTime;
}

Audio * Engine::AudioManager()
{
	return audio;
}

Input * Engine::InputManager()
{
	return input;
}

Renderer * Engine::RenderManager()
{
	return renderer;
}

Window * Engine::WindowManager()
{
	return window;
}

Graphics * Engine::GraphicsManager()
{
	return graphics;
}

Game * Engine::CurrentGame()
{
	return game;
}

float Engine::GetMinFPS()
{
	return minFPS;
}

float Engine::GetAverageFPS()
{
	return averageFPS;
}

float Engine::GetMaxFPS()
{
	return maxFPS;
}

float Engine::PhysicsFrameTime()
{
	return physicsFrameTime;
}

// -----------------------------------------------------------------------------
