#if 0

/**********************************************************************************
// Emitter (Arquivo de Cabeçalho)
// 
// Criação:		07 Out 2012
// Atualização:	14 Jan 2015
// Compilador:	Visual C++ 12
//
// Descrição:	Define um sistema emissor de partículas
//
**********************************************************************************/

#ifndef _DESENVJOGOS_EMITTER_H_
#define _DESENVJOGOS_EMITTER_H_

// ---------------------------------------------------------------------------------

#include "Sprite.h"									// imageSprite formador da partícula
#include "Timer.h"									// controla tempo para geração de partículas
#include "Types.h"									// tipos específicos da engine
#include "Vector3D.h"									// as particulas tem uma direção
#include <vector>									// usando vetores da biblioteca STL	
using std::vector;

// ---------------------------------------------------------------------------------

struct ParticleDesc 
{
	string imgFile;									// nome do arquivo de imagem da partícula
	int    baseAngle;								// direção do emissor de partículas
	int    spread;									// ângulo de espalhamento das partículas
	float  lifeTime;								// tempo de vida das partículas
	float  genTime;									// tempo entre geração de partículas
	float  dimTime;									// porcentagem de vida para começar o desaparecimento
	float  minSpeed;								// velocidade mínima das partículas
	float  maxSpeed;								// velocidade máxima das partículas
	float  r, g, b, a;								// cor das partículas
};

struct Particle
{
	float  x, y;									// posição da partícula
	Vector3D speed;									// direção e velocidade
	llong  timeStamp;								// instante de criação
};

// ---------------------------------------------------------------------------------

class Emitter
{
private:
	Timer    timer;									// controle de tempo
	Image * imageSprite;									// imagem da partícula
	int      baseAngle;								// direção do emissor de partículas
	int      spread;								// ângulo de espalhamento das partículas
	float    lifeTime;								// tempo de vida
	float    genTime;								// tempo entre geração de partículas
	float	 dimTime;								// porcentagem de vida para começar o desaparecimento
	float    minSpeed;								// velocidade mínima das partículas
	float    maxSpeed;								// velocidade máxima das partículas
	float    r, g, b, a;							// cor RGBA da partícula
	vector<Particle*> particles;					// vetor de partículas	
	
	void GenParticle(float posX, float posY);		// gera uma partícula

public:
	Emitter(const ParticleDesc& desc);				// construtor
	Emitter(const ParticleDesc& desc, Image * precachedSprite);				// construtor
	~Emitter();										// destrutor

	int  Size();									// número de partículas
	bool Inactive();								// partículas estão inativas?
	void Generate(float x, float y, int num = 1);	// gera novas partículas
	void Update(float deltaT);						// atualiza posição das partículas por deltaT
	void Draw(float z, float scaleFactor = 0.0f);	// desenha partículas		
}; 

// ---------------------------------------------------------------------------------

inline int Emitter::Size()
{ return particles.size(); }

inline bool Emitter::Inactive()
{ return (particles.size() == 0); }

// ---------------------------------------------------------------------------------

#endif

#endif