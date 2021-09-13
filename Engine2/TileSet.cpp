/**********************************************************************************
// TileSet (Código Fonte)
// 
// Criação:		17 Abr 2012
// Atualização: 27 Mai 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Classe para representar uma folha de sprites
//
**********************************************************************************/

#include "TileSet.h"
#include "Image.h"

// -------------------------------------------------------------------------------

TileSet::TileSet(string filename, uint tileWidth, uint tileHeight, uint colsCount, uint tileCount) : 
	image(new Image(filename)), 
	width(tileWidth), 
	height(tileHeight), 
	columns(colsCount),	
	size(tileCount)
{
	
}

// -------------------------------------------------------------------------------

TileSet::TileSet(string filename, uint lineCount, uint colsCount) :
image(new Image(filename)),
columns(colsCount),
size(lineCount * colsCount)
{
	width  = image->Width() / colsCount;
	height = image->Height() / lineCount;
}

// -------------------------------------------------------------------------------

TileSet::~TileSet()
{
	// libera memória ocupada pela imagem
	if (image)
	{
		 delete image;
		 image = nullptr;
	}
}

// -------------------------------------------------------------------------------

SpriteInfo * TileSet::GetSprite(uint aFrame)
{
	SpriteInfo * spriteRet = new SpriteInfo;

	spriteRet->width = TileWidth();
	spriteRet->height = TileHeight();
	spriteRet->texSize.x = float(TileWidth()) / Width();
	spriteRet->texSize.y = float(TileHeight()) / Height();
	spriteRet->texCoord.x = (aFrame % Columns()) * spriteRet->texSize.x;
	spriteRet->texCoord.y = (aFrame / Columns()) * spriteRet->texSize.y;
	spriteRet->texture = View();

	return spriteRet;
}

// -------------------------------------------------------------------------------