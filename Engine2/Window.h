/**********************************************************************************
// Window (Arquivo de Cabe�alho)
// 
// Cria��o:		19 Mai 2007
// Atualiza��o:	20 Jun 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	A classe abstrai todos os detalhes de configura��o de
//              uma janela para um jogo. 
//
**********************************************************************************/

#ifndef _DESENVJOGOS_WINDOW_H_
#define _DESENVJOGOS_WINDOW_H_

// ---------------------------------------------------------------------------------
// Inclus�es

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>	// inclui fun��es do windows
#include <windowsx.h>	// inclui extens�es do windows
#include "Types.h"      // tipos personalizados do motor
#include <string>		// include a classe string
#include <mutex>
#include <queue>
using std::pair;
using std::queue;
using std::mutex;
using std::unique_lock;
using std::string;		// permite usar o tipo string sem std::

// ---------------------------------------------------------------------------------
// Constantes globais e enumera��es

enum WindowModes {FULLSCREEN, WINDOWED};

typedef pair<BYTE, bool> KeyboardData;

// ---------------------------------------------------------------------------------

class Window
{
private:
	HINSTANCE	hInstance;			// identificador da aplica��o
	HWND		windowHandle;		// identificador da janela
	int			windowWidth;		// largura da janela
	int			windowHeight;		// altura da janela
	HICON		windowIcon;			// �cone da janela
	HCURSOR		windowCursor;		// cursor da janela
	COLORREF	windowColor;		// cor de fundo da janela
	string		windowTitle;		// nome da barra de t�tulo
	DWORD		windowStyle;		// estilo da janela 
	int			windowMode;			// modo tela cheia, em janela ou sem borda
	int			windowPosX;			// posi��o inicial da janela no eixo x
	int			windowPosY;			// posi��o inicial da janela no eixo y
	float		windowCenterX;		// centro da janela no eixo x
	float		windowCenterY;		// centro da janela no eixo y
	bool        windowCtrl[256];	// controle da libera��o de teclas

	int			internalWidth;
	int			internalHeight;

	queue<KeyboardData>	keyboardMessages;
	int			keyboardUnreadMessages;
	bool		mouseLB;
	int			mouseWheelDirection;
	mutex		mouseButtonMutex;
	mutex		mouseWheelMutex;
	mutex		keyboardMutex;
	
public:
	Window();												// construtor de Window

	HINSTANCE AppId();										// retorna o identificador da aplica��o
	HWND Id();												// retorna o identificador da janela
	int  Width(); 											// retorna a largura atual da janela
	int  Height(); 											// retorna a altura atual da janela
	int  InternalWidth();
	int  InternalHeight();

	void InternalWidth(int value);
	void InternalHeight(int value);

	void InternalResolution(int width, int height);

	void Icon(const uint icon);								// define o �cone da janela
	void Cursor(const uint cursor);							// define o cursor da janela
	void Title(const string title);							// define o t�tulo da janela 
	void Size(int width, int height);						// define o tamanho (largura e altura) da janela
	void Mode(int mode);									// define o modo da janela (FULLSCREEN/WINDOWED)
 
	int  Mode() const; 										// retorna o modo atual da janela (FULLSCREEN/WINDOWED)
	float CenterX() const;									// retorna o centro da janela no eixo x
	float CenterY() const;									// retorna o centro da janela no eixo y
	string Title() const;									// retorna t�tulo da janela
	
	void HideCursor(bool hide);								// habilita ou desabilita a exbi��o do cursor
	void CloseOnEscape();									// fecha a janela pelo pressionamento da tecla ESC
	void Close();											// fecha janela imediatamente

	void Resize(int width, int height);
	bool KeyCtrl(int vkcode, bool pressed);					// registra pressionamente somente ap�s libera��o

	POINT Mouse(); 											// retorna posi��o do mouse
	void MouseLeftButton(bool pressed);
	bool MouseLeftButton();
	int MouseWheel();
	void MouseWheel(int direction);

	KeyboardData KeyboardMessage();

	COLORREF Color();										// retorna a cor de fundo da janela
	void Color(int r, int g, int b);						// define a cor de fundo da janela
	void Print(string text, int x, int y, COLORREF color);	// mostra texto usando a GDI do Windows (lento)
	bool Initialize();										// registra e cria a janela

	// tratamento de eventos do Windows
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
};

// ---------------------------------------------------------------------------------

// Fun��es Membro Inline

// retorna identificador da aplica��o
inline HINSTANCE Window::AppId()
{ return hInstance; }

// retorna o identificador da janela do jogo
inline HWND Window::Id()
{ return windowHandle; }

// retorna a largura atual da janela
inline int Window::Width() 
{ return windowWidth;  }

// retorna a altura atual da janela
inline int Window::Height() 
{ return windowHeight; }

inline int Window::InternalWidth()
{ return internalWidth; }

inline int Window::InternalHeight()
{ return internalHeight; }

inline void Window::InternalWidth(int value)
{ internalWidth = value; }

inline void Window::InternalHeight(int value)
{ internalHeight = value; }

inline void Window::InternalResolution(int width, int height)
{
	internalWidth = width;
	internalHeight = height;
}

// ----------------------------------------------------------

// define o �cone da janela
inline void Window::Icon(const uint icon)	
{ windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon)); }

// define o cursor da janela
inline void Window::Cursor(const uint cursor)
{ windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor)); }

// define o t�tulo da janela 
inline void Window::Title(const string title)
{ windowTitle = title; }

// -----------------------------------------------------------

// retorna o modo atual da janela (FULLSCREEN/WINDOWED)
inline int Window::Mode() const 
{ return windowMode;   }

// retorna o centro da janela no eixo horizontal
inline float Window::CenterX() const
{ return windowCenterX; }

// retorna o centro da janela no eixo vertical
inline float Window::CenterY() const
{ return windowCenterY; }

// retorna t�tulo da janela
inline string Window::Title() const
{ return windowTitle; }

// -----------------------------------------------------------

// habilita ou desabilita a exbi��o do cursor
inline void Window::HideCursor(bool hide)
{ ShowCursor(!hide); }

// fecha a janela pelo pressionamento da tecla ESC
inline void Window::CloseOnEscape()
{ //if (KeyDown(VK_ESCAPE)) PostMessage(windowHandle, WM_DESTROY,0,0); 
}

// fecha a janela imediatamente 
inline void Window::Close()
{ PostMessage(windowHandle, WM_DESTROY,0,0); }

// -----------------------------------------------------------

// retorna a cor de fundo da janela
inline COLORREF Window::Color()	
{ return windowColor; }

// define a cor de fundo da janela
inline void Window::Color(int r, int g, int b)	
{ windowColor = RGB(r,g,b); }

// ---------------------------------------------------------------------------------


#endif