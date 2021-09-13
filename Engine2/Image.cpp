/**********************************************************************************
// Image (Código Fonte)
// 
// Criação:		16 Mar 2012
// Atualização:	23 Jul 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Define uma classe para representar imagens
//
**********************************************************************************/

#include "Image.h"
#include "Graphics.h"
#include "Engine.h"

// -------------------------------------------------------------------------------

Image::Image(ID3D11Resource** texture, uint width, uint height)
{
	textureView = nullptr;
	*texture = UpdateTexture(width, height);
}

Image::Image(string filename)
{
	textureView = nullptr;

	// cria sharer resource view da imagem em disco
	D3D11CreateTextureFromFile(
		Graphics::Device(),			// dispositivo Direct3D
		Graphics::Context(),		// contexto do dispositivo
		filename.c_str(),			// nome do arquivo de imagem
		nullptr,					// retorna textura 
		&textureView,				// retorna view da textura
		width,						// retorna largura da imagem
		height);					// retorna altura da imagem
}

// -------------------------------------------------------------------------------

Image::Image(string filename, uint tileWidth, uint tileHeight, uint numColumns, 
	 int * tileMap, uint tileMapCols, uint tileMapRows)
{
}

// -------------------------------------------------------------------------------

Image::~Image()
{
	if (textureView)
	{
		// pega ponteiro para recurso
		ID3D11Resource * resource = nullptr;

		for (size_t i = 0; i < 2; i++)
		{
			if (textureView[i])
			{
				textureView[i]->GetResource(&resource);

				// liberando a view não libera automaticamente
				// o recurso que foi criado junto com a view
				if (resource)
				{
					resource->Release();
					resource = nullptr;
				}

				textureView[i]->Release();
			}
		}

		textureView = nullptr;
	}
}

ID3D11Resource * Image::UpdateTexture(uint width, uint height)
{
	if (!textureView)
	{
		textureView = new ID3D11ShaderResourceView*[views];
		textureView[0] = nullptr;
		textureView[1] = nullptr;
	}

	D3D11CreateTextureFromMemory(
		Graphics::Device(),
		Graphics::Context(),
		&mapTexture,
		&textureView[0],
		&textureView[1],
		width,
		height);

	this->width = width;
	this->height = height;

	return mapTexture;
}

// -------------------------------------------------------------------------------

void Image::DrawWindow(float x, float y, float z, float scale, float rotation, Color & color)
{
	SpriteInfo * spriteInfo = new SpriteInfo;

	spriteInfo->x = x;
	spriteInfo->y = y;
	spriteInfo->scale = scale;
	spriteInfo->depth = z;
	spriteInfo->rotation = rotation * XM_PI / 180.0f;
	spriteInfo->width = Width();
	spriteInfo->height = Height();
	spriteInfo->color = color;
	spriteInfo->texture = View();

	// adiciona o imageSprite na lista de desenho
	Engine::RenderManager()->Draw(*spriteInfo);
}

// -------------------------------------------------------------------------------

void Image::DrawWorld(float x, float y, float z, float scale, float rotation, Color & color)
{
	SpriteInfo * spriteInfo = new SpriteInfo;

	// ajusta coordenadas para viewport
	x -= Game::viewport.left;
	y -= Game::viewport.top;

	spriteInfo->x = x;
	spriteInfo->y = y;
	spriteInfo->scale = scale;
	spriteInfo->depth = z;
	spriteInfo->rotation = rotation * XM_PI / 180.0f;
	spriteInfo->width = Width();
	spriteInfo->height = Height();
	spriteInfo->color = color;
	spriteInfo->texture = View();

	// adiciona o imageSprite na lista de desenho
	Engine::RenderManager()->Draw(*spriteInfo);
}