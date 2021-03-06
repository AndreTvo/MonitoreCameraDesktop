/**********************************************************************************
// Animation (Arquivo de Cabe?alho)
// 
// Cria??o:		28 Set 2011
// Atualiza??o:	29 Mai 2014
// Compilador:	Visual C++ 12
//
// Descri??o:	Classe para animar seq??ncias em folha de sprites
//
**********************************************************************************/

#ifndef _DESENVJOGOS_ANIMATION_H_
#define _DESENVJOGOS_ANIMATION_H_

// ---------------------------------------------------------------------------------

#include "TileSet.h"											// folha de sprites da anima??o
#include "Timer.h"												// tempo entre quadros da anima??o
#include "Types.h"												// tipos espec?ficos da engine
#include "Component.h"

#include <unordered_map>										// biblioteca STL				
using std::unordered_map;										// usando tabelas de dispers?o
using std::pair;												// usando pares de elementos

typedef pair<uint*, uint> AnimSeq;								// seq??ncia <vetor, tam>
typedef unordered_map<uint, AnimSeq> HashTable;					// tabela de dispers?o <uint, AnimSeq>

// ---------------------------------------------------------------------------------

class Animation : public Component
{
private:
	uint iniFrame;												// quadro inicial da seq??ncia
	uint endFrame;												// quadro final da seq??ncia
	uint idSequence;											// identificador da sequencia atual
	uint  frame;												// quadro atual da anima??o
	bool animLoop;												// anima??o em loop infinito
	float animDelay;											// espa?o de tempo entre quadros (em segundos)
	Timer timer;												// medidor de tempo entre quadros da anima??o
	TileSet * tileSet;											// ponteiro para folha de sprites da anima??o
	SpriteInfo sprite;											// sprite a ser desenhada
	HashTable table;											// tabela com seq??ncias de anima??o
	uint * sequence;											// seq??ncia atualmente selecionada

	float depth;
	float scale;
	float rotation;

public:
	~Animation();												// destrutor

	bool Init();
	void PostInit();

	void Update(float gameTime);
	void Draw();

	void SetTileSet(TileSet * tiles);
	void Repeat(bool value);
	void Delay(float value);
	void Depth(float value);
	void Scale(float value);
	void Rotation(float value);

	void Add(uint id, uint * seq, uint seqSize);				// adiciona seq??ncia de anima??o
	void Select(uint id);										// seleciona seq??ncia atual

	void Draw(													// desenha o quadro atual da anima??o
		float x, float y, float z = 0.5f,						// coordenadas da tela
		float scale = 1.0f, float rotation = 0.0f,				// escala e rota??o
		Color color = { 1, 1, 1, 1 });							// efeito de cor

	void Draw(													// desenha um quadro da folha de sprites
		uint aFrame,											// quadro da folha a desenhar
		float x, float y, float z = 0.5f,						// coordenadas da tela
		float scale = 1.0f, float rotation = 0.0f,				// escala e rota??o
		Color color = { 1, 1, 1, 1 });							// efeito de cor

	void Frame(uint aFrame);									// define o frame atual da anima??o
	uint Frame();												// retorna o frame de anima??o ativo

	uint SelectedId() const;
	bool Inactive();											// verifica se a anima??o j? encerrou
	void NextFrame();											// passa para o pr?ximo frame da anima??o
	void Restart();												// reinicia a animac?o (pelo primeiro frame da seq??ncia)
}; 

// ---------------------------------------------------------------------------------
// fun??es membro inline

// desenha quadro atual da anima??o
inline void Animation::Draw(float x, float y, float z, float scale, float rotation, Color color)
{ sequence ? Draw(sequence[frame], x, y, z, scale, rotation, color) : Draw(frame, x, y, z, scale, rotation, color); }

// define o frame atual da anima??o
inline void Animation::Frame(uint aFrame)
{ frame = aFrame; }

// retorna o frame de anima??o ativo
inline unsigned Animation::Frame()
{ return (sequence ? sequence[frame] : frame); }

inline uint Animation::SelectedId() const
{
	return idSequence;
}

// verifica se a anima??o j? encerrou
inline bool Animation::Inactive()
{ return !animLoop && (frame > endFrame || (frame == endFrame && timer.Elapsed(animDelay))); }

// reinicia a animac?o (pelo primeiro frame da sequencia)
inline void Animation::Restart()
{ frame = 0; timer.Start(); }

// ---------------------------------------------------------------------------------

#endif