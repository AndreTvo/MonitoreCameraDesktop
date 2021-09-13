/**********************************************************************************
// Image (Arquivo de Cabe�alho)
// 
// Cria��o:		16 Mar 2012
// Atualiza��o:	23 Jul 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Define uma classe para representar imagens
//
**********************************************************************************/

#ifndef _DESENVJOGOS_IMAGE_H_
#define _DESENVJOGOS_IMAGE_H_

// ---------------------------------------------------------------------------------
// Inclus�es

#include "Types.h"		// tipos espec�ficos do motor
#include "Texture.h"	// fun��o para carregar textura
#include <string>		// classe string de C++
using std::string;		// classe pode ser usada sem std::

// ---------------------------------------------------------------------------------

class Image
{
private:
	ID3D11Resource * mapTexture;
	ID3D11ShaderResourceView ** textureView;	// view associada a textura
	uint width;									// altura da imagem
	uint height;								// largura da imagem
	uint views;

public:
	Image(ID3D11Resource** texture, uint width, uint height);

	Image(string filename);						// constroi imagem a partir de um arquivo

	Image(string filename,						// arquivo que cont�m os blocos de imagem
		uint tileWidth,							// largura dos blocos
		uint tileHeight,						// altura dos blocos
		uint numColumns,						// n�mero de colunas 
		int * tileMap,							// matriz para montagem dos blocos
		uint tileMapCols,						// n�mero de colunas da matriz
		uint tileMapRows);						// n�mero de linhas da matriz

	~Image();									// destrutor

	ID3D11Resource * UpdateTexture(uint width, uint height);

	uint Width() const;							// retorna largura da imagem
	uint Height() const;						// retorna altura da imagem
	ID3D11ShaderResourceView ** View() const;	// retorna ponteiro para a view da imagem

	void DrawWindow(								// desenha imageSprite
		float x, float y, float z = 0.5f,			// coordenadas da tela
		float scale = 1.0f, float rotation = 0.0f,  // escala e rota��o
		Color & color = Color(1, 1, 1, 1));			// efeito de cor

	void DrawWorld(									// desenha imageSprite
		float x, float y, float z = 0.5f,			// coordenadas da tela
		float scale = 1.0f, float rotation = 0.0f,  // escala e rota��o
		Color & color = Color(1, 1, 1, 1));			// efeito de cor

};

// ---------------------------------------------------------------------------------
// M�todos Inline

// retorna largura da textura
inline uint Image::Width() const
{ return width; }

// retorna altura da textura
inline uint Image::Height() const
{ return height; }

// retorna ponteiro para textura D3D
inline ID3D11ShaderResourceView ** Image::View() const
{ return textureView; }

// --------------------------------------------------------------------------------

#endif




