/**********************************************************************************
// Types (Arquivo de Cabeçalho)
// 
// Criação:		02 Abr 2011
// Atualização:	22 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Isola os tipos específicos da plataforma fornecendo tipos
//				consistentes para o Motor de Jogo.
//
**********************************************************************************/

#ifndef _DESENVJOGOS_TYPES_H_
#define _DESENVJOGOS_TYPES_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

// nomes para tipos padrões da linguagem C++ usados na engine
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned long		ulong;
typedef long long			llong;
typedef unsigned long long	ullong;
typedef unsigned int		uint;
typedef unsigned char		byte;

// tipos de objeto
enum ObjectType
{
	CONFIG_BLOCK_SPRITE = 111,
	CONFIG_WORLD_SIZE = 333,
	MOUSE = 99,
	MISSILE = 50,
	PLAYER = 1,
	PLATFORM,
	PORTAL,
	ELEVATOR_POINT,
	BLOCK
};

// formatos geométricos
enum GeometryTypes
{
	UNKNOWN_T,
	POINT_T,
	LINE_T,
	RECTANGLE_T,
	CIRCLE_T,
	POLYGON_T,
	MULTI_T
};

enum GeometryState
{
	MOVING_T,
	STATIC_T
};

// vértice para o Direct3D
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 tex;
};

// definição de cor para a engine
struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color() { r = 0; g = 0; b = 0; a = 0; }

	Color(float red, float green, float blue, float alpha) : 
		r(red), g(green), b(blue), a(alpha){}
};

// informações de um imageSprite
struct SpriteInfo
{
	float x, y;
	float scale, depth, rotation;
	uint  width, height;
	ID3D11ShaderResourceView ** texture;
	XMFLOAT2 texCoord;
	XMFLOAT2 texSize;
	Color color;

	SpriteInfo() :
		x(0), y(0), 
		scale(0), depth(0), rotation(0), 
		width(0), height(0), 
		texture(nullptr), 
		texCoord(XMFLOAT2(0,0)),
		texSize(XMFLOAT2(1,1)),
		color(Color(1,1,1,1)){}
};

// tipo de objeto
enum ObjectStatus
{
	STATIC,
	MOVING
};

struct ViewPort
{
	float left;
	float top;
	float right;
	float bottom;
};

#endif