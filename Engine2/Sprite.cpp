/**********************************************************************************
// Sprite (Código Fonte)
// 
// Criação:		11 Jul 2007
// Atualização:	23 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Define uma classe para representar um imageSprite
//
**********************************************************************************/

#include "Sprite.h"
#include "Object.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

Sprite::~Sprite()
{
	if (localImage)
		delete image;
}

bool Sprite::Init()
{
	image = nullptr;
	enabled = true;
	worldDraw = true;
	alignX = 0;
	alignY = 0;

	scale = 1;
	rotation = 0;
	color = Color(1, 1, 1, 1);

	return true;
}

void Sprite::PostInit()
{
	Position * position = pOwner->Components()->GetComponent<Position>();	
	vector = position->Vector();
}

void Sprite::Update(float gameTime)
{
}

void Sprite::Draw()
{
	float x = vector->X() + alignX;
	float y = vector->Y() + alignY;
	float depth = vector->Depth();

	if (worldDraw)
	{
		// ajusta coordenadas para viewport
		x -= Game::viewport.left;
		y -= Game::viewport.top;
	}

	spriteInfo.x = x;
	spriteInfo.y = y;
	spriteInfo.scale = scale;
	spriteInfo.depth = depth;
	spriteInfo.rotation = rotation * XM_PI / 180.0f;

	if (image)
	{
		spriteInfo.width = image->Width();
		spriteInfo.height = image->Height();
	}

	spriteInfo.color = color;

	// adiciona o imageSprite na lista de desenho
	Engine::RenderManager()->Draw(spriteInfo);
}

void Sprite::LoadImageFile(string filename)
{
	// carrega imagem
	image = new Image(filename);
	localImage = true;

	// configura registro imageSprite
	spriteInfo.texture = image->View();
}

void Sprite::LoadImageFile(const Image * img)
{
	// aponta para imagem externa
	image = img;
	localImage = false;

	// configura registro imageSprite
	spriteInfo.texture = image->View();
}

void Sprite::LoadSpriteInfo(SpriteInfo * spriteInfo)
{
	image = nullptr;
	this->spriteInfo = *spriteInfo;
}

void Sprite::SetScale(float scale)
{
	this->scale = scale;
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;
}

void Sprite::SetColor(Color color)
{
	this->color = color;
}

float Sprite::GetScale() const
{
	return scale;
}

float Sprite::GetRotation() const
{
	return rotation;
}

Color Sprite::GetColor() const
{
	return color;
}

void Sprite::AlignX(float value)
{
	alignX = value;
}

void Sprite::AlignY(float value)
{
	alignY = value;
}

bool Sprite::WorldDraw() const
{
	return worldDraw;
}

void Sprite::WorldDraw(bool value)
{
	this->worldDraw = value;
}

// retorna a largura do imageSprite
int Sprite::Width() const
{
	return image->Width();
}

// retorna a altura do imageSprite
int Sprite::Height() const
{
	return image->Height();
}

// ---------------------------------------------------------------------------------