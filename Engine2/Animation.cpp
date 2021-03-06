/**********************************************************************************
// Animation (C?digo Fonte)
// 
// Cria??o:		28 Set 2011
// Atualiza??o:	25 Jul 2014
// Compilador:	Visual C++ 12
//
// Descri??o:	Classe para animar seq??ncias em folha de sprites
//
**********************************************************************************/

#include "Engine.h"
#include "Animation.h"
#include "Renderer.h"
#include "Object.h"

// --------------------------------------------------------------------------------

Animation::~Animation()
{
	if (!table.empty())
	{
		for (auto i = table.begin(); i != table.end(); i++)
			delete i->second.first;

		table.clear();
	}
}

bool Animation::Init()
{
	// sempre inicia a anima??o pelo primeiro quadro
	frame = iniFrame = 0;

	// nenhuma seq??ncia selecionada
	sequence = nullptr;

	depth = 0.5f;
	scale = 1;
	rotation = 0;

	enabled = true;

	return true;
}

void Animation::PostInit()
{
}

void Animation::Update(float gameTime)
{
	NextFrame();
}

void Animation::Draw()
{
	Draw(pOwner->X(), pOwner->Y(), depth, scale, rotation);
}

void Animation::SetTileSet(TileSet * tiles)
{
	tileSet = tiles;

	// o ?ltimo quadro ? sempre um a menos que o n?mero de quadros
	endFrame = tileSet->Size() - 1;

	// configura imageSprite
	sprite.scale = 1.0f;
	sprite.rotation = 0.0f;
	sprite.width = tileSet->TileWidth();
	sprite.height = tileSet->TileHeight();
	sprite.texSize.x = float(tileSet->TileWidth()) / tileSet->Width();
	sprite.texSize.y = float(tileSet->TileHeight()) / tileSet->Height();
	sprite.texture = tileSet->View();

	// anima??o iniciada (come?a a contar o tempo)
	timer.Start();
}

void Animation::Repeat(bool value)
{
	animLoop = value;
}

void Animation::Delay(float value)
{
	animDelay = value;
}

void Animation::Depth(float value)
{
	depth = value;
}

void Animation::Scale(float value)
{
	scale = value;
}

void Animation::Rotation(float value)
{
	rotation = value;
}

// --------------------------------------------------------------------------------

void Animation::Add(uint id, uint * seq, uint seqSize)
{
	// cria nova seq??ncia de anima??o
	AnimSeq newSeq(new uint[seqSize], seqSize);

	// copia vetor com a seq??ncia de quadros da anima??o
	memcpy(newSeq.first, seq, sizeof(uint) * seqSize);

	// insere nova seq??ncia
	table[id] = newSeq;

	// seleciona seq??ncia rec?m inserida
	sequence = newSeq.first;

	// inicia anima??o pelo primeiro quadro da seq??ncia
	iniFrame = frame = 0;

	// o ?ltimo quadro ? sempre um a menos que o n?mero de quadros
	endFrame = seqSize - 1;
}

// --------------------------------------------------------------------------------

void Animation::Select(uint id)
{
	// se uma nova sequ?ncia for selecionada
	if (sequence != table[id].first)
	{
		idSequence = id;

		// seleciona a seq??ncia pelo seu identificador
		sequence = table[id].first;

		// se a seq??ncia mudou reinicia a sequ?ncia
		iniFrame = 0;
		endFrame = table[id].second - 1;

		// se o frame atual est? fora da sequ?ncia
		//if (frame > endFrame)
			frame = 0;
	}
}


// ---------------------------------------------------------------------------------

void Animation::NextFrame()
{
	// passa para o pr?ximo quadro ap?s espa?o de tempo estipulado em animDelay
	if (timer.Elapsed(animDelay))
	{
		frame++;

		// volta para o primeiro quadro se estiver usando o modo ENDLESS
		if (animLoop && frame > endFrame)
			frame = 0;

		// frame iniciado (come?a a contar o tempo)
		timer.Start();		
	}
}

// ---------------------------------------------------------------------------------

void Animation::Draw(uint aFrame, float x, float y, float z, float scale, float rotation, Color color)
{
	// ajusta coordenadas para viewport
	x -= Game::viewport.left;
	y -= Game::viewport.top;

	// configura dados b?sicos
	sprite.x = x;
	sprite.y = y;
	sprite.depth = z;
	sprite.scale = scale;
	sprite.rotation = rotation * XM_PI / 180.0f;

	// configura coordenadas da textura do imageSprite
	sprite.texCoord.x = (aFrame % tileSet->Columns()) * sprite.texSize.x;
	sprite.texCoord.y = (aFrame / tileSet->Columns()) * sprite.texSize.y;

	// adiciona o imageSprite na lista de desenho
	Engine::RenderManager()->Draw(sprite);
}

// --------------------------------------------------------------------------------