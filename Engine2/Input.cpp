#include "Input.h"
#include "Engine.h"

Input::Input()
{
	dinput = nullptr;
	joyDev = nullptr;

	joyCtrl[10] = { 0 };
	keyboardCtrl[256] = { 0 };
	xboxSelected = 0;

	genericJoystickEnabled = false;
	keyboardEnabled = false;
	mouseEnabled = false;

	ZeroMemory(&joyState, sizeof(joyState));
	ZeroMemory(&joyCtrl, sizeof(joyCtrl));
	ZeroMemory(&xboxState, sizeof(xboxState));

	// cria o objeto direct input
	DirectInput8Create(
		GetModuleHandle(NULL),		// identificador da janela
		DIRECTINPUT_VERSION,		// versão do direct input
		IID_IDirectInput8,			// identificador da interface
		(void **)&dinput,			// objeto direct input
		NULL);						// sempre nulo

	// busca por dispositivos conectados usando a função callback JoystickEnumerate
	dinput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,		// procure apenas joysticks
		JoystickEnumerate,			// função de enumeração
		&joysticks,					// endereço da lista de joysticks
		DIEDFL_ATTACHEDONLY);		// apenas dispositivos conectados

	if (!joysticks.empty())
	{
		// seleciona e inicializa o primeiro joystick encontrado
		genericSelected = joysticks[0];
	}

	if (&Window::Mouse)
		mouseEnabled = true;
}

Input::~Input()
{
	// libera o dispositivo joystick
	if (joyDev)
		joyDev->Unacquire();

	// apaga o objeto joystick
	if (joyDev)
		joyDev->Release();

	// apaga o objeto direct input
	if (dinput)
		dinput->Release();
}

void Input::Update()
{
	if (genericJoystickEnabled)
		GetState();

	if (xboxJoystickEnabled)
		GetXboxState();
}

// ---------------------------------------------------------------------------------

bool Input::ActivateJoystick()
{
	// se o direct input não foi inicializado saia
	if (!dinput)
		return false;

	// libera o joystick selecionado
	if (joyDev)
		joyDev->Unacquire();

	// apaga o objeto do joystick selecionado
	if (joyDev)
		joyDev->Release();

	// verifica se existe algum joystick no sistema
	if (joysticks.empty())
		return false;

	// cria o dispositivo joystick 
	if (FAILED(dinput->CreateDevice(genericSelected.guid, &joyDev, NULL)))
		return false;

	// seleciona o nível de cooperação para o joystick
	if (FAILED(joyDev->SetCooperativeLevel(Engine::WindowManager()->Id(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	// seleciona o formato de dados
	if (FAILED(joyDev->SetDataFormat(&c_dfDIJoystick)))
		return false;

	// enumera os objetos do joystick selecionado (botões e eixos)
	joystickObjects.clear();
	if (FAILED(joyDev->EnumObjects(JoystickEnumObjects, &joystickObjects, DIDFT_ALL)))
		return false;

	// ajusta a faixa de movimento e a zona morta de um eixo
	DIPROPRANGE joyAxisRange;
	DIPROPDWORD deadZone;

	for (auto it : joystickObjects)
	{
		// se o objeto é um eixo
		if (it.type & DIDFT_AXIS)
		{
			// define faixa do eixo
			joyAxisRange.diph.dwSize = sizeof(DIPROPRANGE);
			joyAxisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			joyAxisRange.diph.dwHow = DIPH_BYID;
			joyAxisRange.diph.dwObj = it.type;
			joyAxisRange.lMin = -1000;
			joyAxisRange.lMax = +1000;

			// aplica a faixa no eixo
			if (FAILED(joyDev->SetProperty(DIPROP_RANGE, &joyAxisRange.diph)))
				return false;

			// define a zona morta
			deadZone.diph.dwSize = sizeof(deadZone);
			deadZone.diph.dwHeaderSize = sizeof(deadZone.diph);
			deadZone.diph.dwHow = DIPH_BYID;
			deadZone.diph.dwObj = it.type;
			deadZone.dwData = 2000;

			// aplica a zona morta no eixo
			if (FAILED(joyDev->SetProperty(DIPROP_DEADZONE, &deadZone.diph)))
				return false;
		}
	}

	// requisita acesso ao joystick
	if (FAILED(joyDev->Acquire()))
		return false;

	genericJoystickEnabled = true;

	// inicialização bem sucedida
	return true;
}

// ---------------------------------------------------------------------------------

bool Input::ActivateXbox()
{
	xboxJoystickEnabled = true;

	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(xboxSelected, XINPUT_FLAG_GAMEPAD, &caps);

	if (caps.SubType != XINPUT_DEVSUBTYPE_GAMEPAD)
		return false;

	return true;
}

// ---------------------------------------------------------------------------------

bool Input::GetState()
{
	assert(genericJoystickEnabled);

	// é preciso usar Poll() antes de ler o estado do joystick
	// porque alguns drivers de joystick não atualizam seu estado
	// interno automaticamente

	// sempre atualiza o estado interno do joystick antes ler seu estado
	if (FAILED(joyDev->Poll()))
		return false;

	// lê o estado do joystick
	HRESULT result = joyDev->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joyState);

	// aconteceu alguma falha na leitura
	if (FAILED(result))
	{
		if (result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
		{
			// libera o dispositivo joystick
			joyDev->Unacquire();

			// tenta re-capturar o joystick
			joyDev->Acquire();
		}
		else
		{
			return false;
		}
	}

	// leitura bem sucedida
	return true;
}

// ---------------------------------------------------------------------------------

bool Input::GetXboxState()
{
	assert(xboxJoystickEnabled);

	DWORD result = XInputGetState(xboxSelected, &xboxState);

	// se o resultado não é zero significa que a leitura falhou
	if (result)
		return false;

	// leitura bem sucedida
	return true;
}

// ---------------------------------------------------------------------------------

bool Input::XboxButtonDown(int button)
{
	assert(xboxJoystickEnabled);

	switch (button)
	{
	case DpadUp: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP); break;
	case DpadDown: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN); break;
	case DpadLeft: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT); break;
	case DpadRight: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT); break;
	case StartButton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_START); break;
	case BackButton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK); break;
	case LeftThumb: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB); break;
	case RightThumb: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB); break;
	case LeftShoulder: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER); break;
	case RightShoulder: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER); break;
	case Abutton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_A); break;
	case Bbutton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_B); break;
	case Xbutton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_X); break;
	case Ybutton: return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_Y); break;
	}

	return false;
}

bool Input::XboxButtonUp(int button)
{
	return !XboxButtonDown(button);
}

// ---------------------------------------------------------------------------------

bool Input::ButtonCtrl(int button)
{
	assert(genericJoystickEnabled);

	if (joyCtrl[button])
	{
		if (ButtonDown(button))
		{
			joyCtrl[button] = false;
			return true;
		}
	}
	else if (ButtonUp(button))
	{
		joyCtrl[button] = true;
	}

	return false;
}

// ---------------------------------------------------------------------------------

bool Input::XboxButtonCtrl(int button)
{
	assert(xboxJoystickEnabled);

	if (joyCtrl[button])
	{
		if (XboxButtonDown(button))
		{
			joyCtrl[button] = false;
			return true;
		}
	}
	else if (XboxButtonUp(button))
	{
		joyCtrl[button] = true;
	}

	return false;
}

long Input::Axis(int axis)
{
	assert(genericJoystickEnabled);

	switch (axis)
	{
	case JoyAxisX:
		return joyState.lX;
	case JoyAxisY:
		return joyState.lY;
	case JoyAxisZ:
		return joyState.lZ;
	case JoyAxisRX:
		return joyState.lRx;
	case JoyAxisRY:
		return joyState.lRy;
	case JoyAxisRZ:
		return joyState.lRz;

	default:
		return 0;
	}
}

long Input::Slider(int slider)
{
	assert(genericJoystickEnabled);

	if (slider < 2)
		return joyState.rglSlider[slider];
	else
		return 0;
}

// ---------------------------------------------------------------------------------

long Input::Pov(int pov)
{
	assert(genericJoystickEnabled);

	if (pov < 4)
		return joyState.rgdwPOV[pov];
	else
		return 0;
}

// ---------------------------------------------------------------------------------

bool Input::SelectGenericJoystick(uint id)
{
	if (id > joysticks.size())
		return false;

	genericSelected = joysticks[id];
	return true;
}

const string Input::JoyName()
{
	return genericSelected.name;
}

int Input::XboxTrigger(int trigger)
{
	assert(xboxJoystickEnabled);

	switch (trigger)
	{
	case LTtrigger: return xboxState.Gamepad.bLeftTrigger; break;
	case RTtrigger: return xboxState.Gamepad.bRightTrigger; break;
	}

	return 0;
}

int Input::XboxThumbStick(int stick)
{
	assert(xboxJoystickEnabled);

	switch (stick)
	{
	case LXthumb:
		if (abs(xboxState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return xboxState.Gamepad.sThumbLX;
		break;
	case LYthumb:
		if (abs(xboxState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return xboxState.Gamepad.sThumbLY;
		break;
	case RXthumb:
		if (abs(xboxState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return xboxState.Gamepad.sThumbRX;
		break;
	case RYthumb:
		if (abs(xboxState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return xboxState.Gamepad.sThumbRY;
		break;
	}

	return 0;
}

void Input::XboxVibrate(int left, int right)
{
	assert(xboxJoystickEnabled);

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
	XInputSetState(xboxSelected, &vibration);
}

void Input::SelectXboxJoystick(uint id)
{
	xboxSelected = id;
}

bool Input::KeyDown(int vkcode)
{
	return ((GetAsyncKeyState(vkcode) & 0x8000) ? true : false);
}

bool Input::KeyUp(int vkcode)
{
	return ((GetAsyncKeyState(vkcode) & 0x8000) ? false : true);
}

bool Input::KeyCtrl(int vkcode)
{
	if (keyboardCtrl[vkcode])
	{
		if (KeyDown(vkcode))
		{
			keyboardCtrl[vkcode] = false;
			return true;
		}
	}
	else if (KeyUp(vkcode))
	{
		keyboardCtrl[vkcode] = true;
	}

	return false;
}

// ---------------------------------------------------------------------------------

BOOL CALLBACK JoystickEnumerate(LPCDIDEVICEINSTANCE lpDDi, LPVOID data)
{
	// registro que guarda nome e GUID do joystick 
	JoyInfo joy;

	// copia identificador do joystick
	joy.guid = lpDDi->guidInstance;

	// copia nome do joystick
	joy.name = lpDDi->tszInstanceName;

	// push joystick into the list
	((vector<JoyInfo>*) data)->push_back(joy);

	// continua enumeração até o fim
	return DIENUM_CONTINUE;
}

// ---------------------------------------------------------------------------------

BOOL CALLBACK JoystickEnumObjects(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data)
{
	// registro que guarda nome, identificador e tipo dos objetos do joystick 
	// (botões, eixos, povs e sliders)
	JoyObjs button;

	// copia o identificador, tipo e nome do objeto
	button.guid = lpDIdoi->guidType;
	button.type = lpDIdoi->dwType;
	button.name = lpDIdoi->tszName;

	// se o tipo do objeto é conhecido, insere-o na lista de objetos do joystick
	if (lpDIdoi->guidType != GUID_Unknown)
		((vector<JoyObjs>*) data)->push_back(button);

	// continua enumeração até o fim
	return DIENUM_CONTINUE;
}