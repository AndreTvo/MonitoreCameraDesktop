#if 0

/**********************************************************************************
// Emitter (Arquivo de Cabe�alho)
// 
// Cria��o:		07 Out 2012
// Atualiza��o:	14 Jan 2015
// Compilador:	Visual C++ 12
//
// Descri��o:	Define um sistema emissor de part�culas
//
**********************************************************************************/

#ifndef _DESENVJOGOS_EMITTER_H_
#define _DESENVJOGOS_EMITTER_H_

// ---------------------------------------------------------------------------------

#include "Sprite.h"									// imageSprite formador da part�cula
#include "Timer.h"									// controla tempo para gera��o de part�culas
#include "Types.h"									// tipos espec�ficos da engine
#include "Vector3D.h"									// as particulas tem uma dire��o
#include <vector>									// usando vetores da biblioteca STL	
using std::vector;

// ---------------------------------------------------------------------------------

struct ParticleDesc 
{
	string imgFile;									// nome do arquivo de imagem da part�cula
	int    baseAngle;								// dire��o do emissor de part�culas
	int    spread;									// �ngulo de espalhamento das part�culas
	float  lifeTime;								// tempo de vida das part�culas
	float  genTime;									// tempo entre gera��o de part�culas
	float  dimTime;									// porcentagem de vida para come�ar o desaparecimento
	float  minSpeed;								// velocidade m�nima das part�culas
	float  maxSpeed;								// velocidade m�xima das part�culas
	float  r, g, b, a;								// cor das part�culas
};

struct Particle
{
	float  x, y;									// posi��o da part�cula
	Vector3D speed;									// dire��o e velocidade
	llong  timeStamp;								// instante de cria��o
};

// ---------------------------------------------------------------------------------

class Emitter
{
private:
	Timer    timer;									// controle de tempo
	Image * imageSprite;									// imagem da part�cula
	int      baseAngle;								// dire��o do emissor de part�culas
	int      spread;								// �ngulo de espalhamento das part�culas
	float    lifeTime;								// tempo de vida
	float    genTime;								// tempo entre gera��o de part�culas
	float	 dimTime;								// porcentagem de vida para come�ar o desaparecimento
	float    minSpeed;								// velocidade m�nima das part�culas
	float    maxSpeed;								// velocidade m�xima das part�culas
	float    r, g, b, a;							// cor RGBA da part�cula
	vector<Particle*> particles;					// vetor de part�culas	
	
	void GenParticle(float posX, float posY);		// gera uma part�cula

public:
	Emitter(const ParticleDesc& desc);				// construtor
	Emitter(const ParticleDesc& desc, Image * precachedSprite);				// construtor
	~Emitter();										// destrutor

	int  Size();									// n�mero de part�culas
	bool Inactive();								// part�culas est�o inativas?
	void Generate(float x, float y, int num = 1);	// gera novas part�culas
	void Update(float deltaT);						// atualiza posi��o das part�culas por deltaT
	void Draw(float z, float scaleFactor = 0.0f);	// desenha part�culas		
}; 

// ---------------------------------------------------------------------------------

inline int Emitter::Size()
{ return particles.size(); }

inline bool Emitter::Inactive()
{ return (particles.size() == 0); }

// ---------------------------------------------------------------------------------

#endif

#endif