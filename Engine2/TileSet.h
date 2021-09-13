/**********************************************************************************
// TileSet (Arquivo de Cabe�alho)
// 
// Cria��o:		17 Abr 2012
// Atualiza��o: 27 Mai 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Classe para representar uma folha de sprites
//
**********************************************************************************/

#ifndef _DESENVJOGOS_TILESET_H_
#define _DESENVJOGOS_TILESET_H_

// ---------------------------------------------------------------------------------
// inclus�es

#include "Types.h"
#include "Image.h"
#include <string>
using std::string;

// ---------------------------------------------------------------------------------

class TileSet 
{
private:
	Image * image;							// imagem da folha de sprites
	uint width;								// largura de um quadro
	uint height;							// altura de um quadro
	uint columns;							// n�mero de colunas da folha
	uint size;								// quantidade de quadros na folha

public:
	TileSet(string filename,				// nome do arquivo
		    uint tileWidth,					// largura de um quadro
			uint tileHeight,				// altura de um quadro
			uint colsCount,					// n�mero de colunas na folha
			uint tileCount);				// quantidade de quadros

	TileSet(string filename,				// nome do arquivo
		uint lineCount,						// n�mero de linhas na folha
		uint colsCount);					// n�mero de colunas na folha

	~TileSet();								

	uint    TileWidth();					// retorna a largura de um quadro
	uint    TileHeight();					// retorna a altura de um quadro
	uint    Size();							// retorna n�mero de quadros na folha
	uint    Columns();						// retorna o n�mero de colunas na folha
	uint    Width();						// retorna a largura da folha
	uint    Height();						// retorna a altura da folha

	// -------------------------------------------------------------------------------

	SpriteInfo * TileSet::GetSprite(uint aFrame);

	// retorna view da textura da folha
	ID3D11ShaderResourceView ** View();		
};

// ---------------------------------------------------------------------------------
// fun��es membro inline

// retorna a largura de um quadro
inline uint TileSet::TileWidth()
{ return width; }

// retorna a altura de um quadro
inline uint TileSet::TileHeight()
{ return height; }

// retorna n�mero de quadros na folha
inline uint TileSet::Size()
{ return size; }

// retorna o n�mero de colunas na folha
inline uint TileSet::Columns()
{ return columns; }

// retorna a largura da folha
inline uint TileSet::Width()
{ return image->Width(); }

// retorna a altura da folha
inline uint TileSet::Height()
{ return image->Height(); }

// retorna a altura do tileset
inline ID3D11ShaderResourceView ** TileSet::View()
{ return image->View(); }

// ---------------------------------------------------------------------------------

#endif




