/**********************************************************************************
// Sprite (Arquivo de Cabeçalho)
// 
// Criação:		11 Jul 2007
// Atualização:	23 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Define uma classe para representar um imageSprite
//
**********************************************************************************/

#ifndef _DESENVJOGOS_SPRITE_H_
#define _DESENVJOGOS_SPRITE_H_

// ---------------------------------------------------------------------------------

#include "Component.h"
#include "Vector2.h"

#include "Image.h"
#include "Types.h"

// ---------------------------------------------------------------------------------

class Sprite : public Component
{
private:
	Vector2 * vector;								// coordenadas da tela
	SpriteInfo    spriteInfo;						// informações do imageSprite 
	bool          localImage;						// imagem local ou externa
	bool		  worldDraw;						// true para jogo e false para janela
	const Image	* image;							// ponteiro para imagem

	float scale;
	float rotation;
	Color color;

	float alignX;
	float alignY;

public:
	~Sprite();										// destrutor do sprite

	bool Init();
	void PostInit();
	void Update(float gameTime);
	void Draw();

	void LoadImageFile(string filename);			// constroi sprite a partir de um arquivo
	void LoadImageFile(const Image * img);			// constroi sprite a partir de imagem existente
	void LoadSpriteInfo(SpriteInfo * spriteInfo);

	void SetScale(float scale);
	void SetRotation(float rotation);
	void SetColor(Color color);

	float GetScale() const;
	float GetRotation() const;
	Color GetColor() const;

	void AlignX(float value);
	void AlignY(float value);

	bool WorldDraw() const;
	void WorldDraw(bool value);

	int Width() const;								// largura do sprite
	int Height() const;								// altura do sprite
};

#endif