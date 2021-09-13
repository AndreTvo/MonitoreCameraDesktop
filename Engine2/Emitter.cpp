#if 0

/**********************************************************************************
// Particles (Código Fonte)
// 
// Criação:		07 Out 2012
// Atualização:	14 Jan 2015
// Compilador:	Visual C++ 12
//
// Descrição:	Define um sistema emissor de partículas
//
**********************************************************************************/

#include "Emitter.h"
#include "Geometry.h"
#include <cmath>

// ---------------------------------------------------------------------------------

Emitter::Emitter(const ParticleDesc& desc)
{
	imageSprite = new Image(desc.imgFile);
	lifeTime = desc.lifeTime;
	genTime = desc.genTime;
	minSpeed = desc.minSpeed;
	maxSpeed = desc.maxSpeed;
	r = desc.r;
	g = desc.g;
	b = desc.b;
	a = desc.a;
	baseAngle = desc.baseAngle;
	spread = desc.spread;
	timer.Start();
}

// ---------------------------------------------------------------------------------

Emitter::Emitter(const ParticleDesc& desc, Image * precachedSprite)
{
	imageSprite = precachedSprite;
	lifeTime = desc.lifeTime;
	genTime = desc.genTime;
	minSpeed = desc.minSpeed;
	maxSpeed = desc.maxSpeed;
	r = desc.r;
	g = desc.g;
	b = desc.b;
	a = desc.a;
	baseAngle = desc.baseAngle;
	spread = desc.spread;
	timer.Start();
}

// ---------------------------------------------------------------------------------

Emitter::~Emitter()
{
	for (auto & i : particles)
		delete i;

	particles.clear();
}

// ---------------------------------------------------------------------------------

void Emitter::GenParticle(float posX, float posY)
{
	Particle * p = new Particle();

	p->x = posX;
	p->y = posY;
	
	int angle = baseAngle + spread - (rand() % (2*spread+1));
	
	if (angle > 360)
		angle -= 360;
	else 
		if (angle < 0)
		angle += 360;
	
	p->speed.Angle(float(angle));	
	// rand() % (maxSpeed-minSpeed) não funciona porque maxSpeed e minSpeed são float
	p->speed.Magnitude(minSpeed + ((rand() % 101) * 0.01f) * (maxSpeed-minSpeed));
	p->timeStamp = timer.TimeStamp();

	particles.push_back(p);
}

// ---------------------------------------------------------------------------------

void Emitter::Generate(float x, float y, int num)
{
	if (num == 1)
	{
		// gera uma partícula por vez
		if (timer.Elapsed(genTime)) 
		{
			GenParticle(x, y);
			timer.Reset();
		}
	} 
	else 
	{	
		// gera todas as partículas de uma vez
		for (int i = 0; i < num; i++)
		{
			GenParticle(x, y);
		}
	}
}

// ---------------------------------------------------------------------------------

void Emitter::Update(float deltaT)
{
	float lifePercent = 0;
	float timeElapsed = 0;

	auto i = particles.begin();
	while (i != particles.end())
	{
		Particle * p = *i;
		p->x += p->speed.X() * deltaT;
		p->y -= p->speed.Y() * deltaT;
		
		timeElapsed = timer.Elapsed(p->timeStamp);
		lifePercent = timeElapsed/lifeTime;

		// torna partícula transparente com o passar do tempo
		if (lifePercent > dimTime)
			a = 1.0f - pow(lifePercent, 3);

		if (timeElapsed > lifeTime)
		{
			// remove partícula sem vida
			delete *i;
			i = particles.erase(i);
		}
		else
		{
			// passa para próxima partícula
			i++;
		}
	}
}

// ---------------------------------------------------------------------------------


void Emitter::Draw(float z, float scaleFactor)
{
	float lifePercent = 0.0f;
	float timeElapsed = 0.0f;
	
	for (auto i = particles.begin(); i != particles.end(); ++i)
	{
		Particle * p = *i;

		timeElapsed = timer.Elapsed(p->timeStamp);
		lifePercent = timeElapsed/lifeTime;

		// escala cresce quadraticamente com o tempo de vida
		float scale = 1.0f + pow(lifePercent, 2) * scaleFactor;

		imageSprite->DrawWorld(p->x, p->y, z, scale, 0, Color(r, g, b, a));
	}	
}

// ---------------------------------------------------------------------------------

#endif