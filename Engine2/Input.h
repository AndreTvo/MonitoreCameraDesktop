#pragma once

#define INITGUID								// força inclusão dos GUIDs do DirectX
#define DIRECTINPUT_VERSION 0x0800				// usa versão 8 do DirectInput

#include <dinput.h>
#include <xinput.h>
#include <vector>
#include <string>
#include "Types.h"
using std::vector;
using std::string;

// TODO:
// Testar/Implementar multiplos controles ao mesmo tempo

// eixos e botões do controle do Xbox
enum XboxCtrl
{
	DpadUp, DpadDown, DpadLeft, DpadRight,  // Dpad
	StartButton, BackButton,				// Buttons
	LeftThumb, RightThumb,					// Buttons
	LeftShoulder, RightShoulder,			// Buttons
	Abutton, Bbutton, Xbutton, Ybutton,		// Buttons
	LTtrigger, RTtrigger,					// Trigger   = faixa entre 0 e 255
	LXthumb, LYthumb, RXthumb, RYthumb		// Tumbstick = faixa entre -32768 e 32767
};

// eixos do joystick
enum JoyAxis {
	JoyAxisX,
	JoyAxisY,
	JoyAxisZ,
	JoyAxisRX,
	JoyAxisRY,
	JoyAxisRZ
};

// informações do joystick
struct JoyInfo {
	GUID   guid;
	string name;
};

// objetos do joystick (botões e eixos)
struct JoyObjs {
	GUID   guid;
	DWORD  type;
	string name;
};

class Input
{
private:
	LPDIRECTINPUT8 dinput;				// objeto direct input
	LPDIRECTINPUTDEVICE8 joyDev;		// dispositivo do joystick
	DIJOYSTATE joyState;				// estado do joystick
	XINPUT_STATE xboxState;				// estado do controle do Xbox

	bool joyCtrl[10];					// controle da liberação de botões
	bool keyboardCtrl[256];				// controle da liberação de teclas do teclado

	vector<JoyInfo> joysticks;			// lista dos dispositivos tipo joystick
	vector<JoyObjs> joystickObjects;	// lista de botões e eixos para o joystick selecionado

	bool genericJoystickEnabled;		// controle de compatibilidade com joystick generico
	bool xboxJoystickEnabled;			// controle de compatibilidade com joystick xbox
	bool keyboardEnabled;				// controle de compatibilidade com teclado
	bool mouseEnabled;					// controle de compatibilidade com mouse

	JoyInfo genericSelected;			// joystick generico selecionado
	int xboxSelected;					// joystick xbox selecionado

public:
	Input();
	~Input();

	bool ActivateJoystick();					// inicializa joystick selecionado
	bool GetState();							// atualiza estado do joystick
	bool ButtonDown(int button);				// verifica se um botão do joystick está pressionado
	bool ButtonUp(int button);					// verifica se um botão do joystick está liberado
	bool ButtonCtrl(int button);				// registra novo pressionamento somente após liberação
	long Axis(int axis); 						// retorna o movimento de um eixo do joystick
	long Slider(int slider);					// retorna movimento do slider
	long Pov(int pov);							// retorna o movimento do D-pad
	bool SelectGenericJoystick(uint id);		// seleciona joystick
	const string JoyName();						// retorna o nome do joystick selecionado

	bool ActivateXbox();						// ativa funções de xbox e verifica se o controle atual é um xbox
	bool GetXboxState();						// atualiza estado do controle do Xbox
	bool XboxButtonDown(int button);			// verifica se um botão do joystick está pressionado
	bool XboxButtonUp(int button);				// verifica se um botão do joystick está liberado
	bool XboxButtonCtrl(int button);			// registra novo pressionamento somente após liberação
	int  XboxTrigger(int trigger);				// retorna valor dos gatilhos do contole do Xbox
	int  XboxThumbStick(int stick);				// retorna valor dos direcionais do controle do Xbox
	void XboxVibrate(							// faz o controle do xbox vibrar
		int left = 65535, int right = 65535);
	void SelectXboxJoystick(uint id);			// seleciona joystick xbox

	bool KeyDown(int vkcode);					// verifica se uma tecla/botão está pressionado
	bool KeyUp(int vkcode);						// verifica se uma tecla/botão está liberado
	bool KeyCtrl(int vkcode);					// registra pressionamente somente após liberação

	void Update();

	vector<JoyObjs>&	Objects();					// retorna lista de botões e eixos do joystick

	
};

// -------------------------------------------------------------------------------

// funções membro InLine

// verifica se o botão do joystick está pressionado
inline bool Input::ButtonDown(int button)
{
	return joyState.rgbButtons[button] && 0x80;
}

// verifica se o botão do joystick está pressionado
inline bool Input::ButtonUp(int button)
{
	return !(joyState.rgbButtons[button] && 0x80);
}

// retorna lista de botões e eixos do joystick 
inline vector<JoyObjs>& Input::Objects()
{
	return joystickObjects;
}

// ---------------------------------------------------------------------------------

// função callback que enumera os joysticks
BOOL CALLBACK JoystickEnumerate(LPCDIDEVICEINSTANCE lpDDi, LPVOID data);

// função callback que enumera os botões e eixos do joystick
BOOL CALLBACK JoystickEnumObjects(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data);

// ---------------------------------------------------------------------------------