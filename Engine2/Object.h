/**********************************************************************************
// Object (Arquivo de Cabe�alho)
//
// Cria��o:		01 Out 2007
// Atualiza��o:	14 Mai 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Essa � a classe base para todos objetos do jogo.
//
//				Um objeto do jogo � qualquer coisa que poss�mos querer desenhar
//				ou interagir dentro do jogo. Por exemplo, um soldado, um pr�dio,
//				um proj�til, uma forma geom�trica, etc.
//
**********************************************************************************/

#ifndef _DESENVJOGOS_OBJECT_H_
#define _DESENVJOGOS_OBJECT_H_

// -----------------------------------------------------------------------------

#include "Types.h"			// usando os tipos personalizados da engine
#include "Engine.h"
#include "Container.h"

// -----------------------------------------------------------------------------

class Object
{
protected:
	Position * position;
	Container * container;

	uint type;				// tipo do objeto

public:
	Object();				// construtor padr�o de um objeto
	virtual ~Object();		// destrutor virtual

	Container * Components() const;

	// ------------------------
	// fun��es virtuais	
	// ------------------------
	// podem ser sobrescritas na classe derivada
	// e chamadas corretamente mesmo usando um
	// ponteiro para a classe base

	void X(float value);
	float X() const;

	void Y(float value);
	float Y() const;

	float Depth() const;

	void Translate(float dx, float dy);
	void Translate(Vector2 vec);

	void MoveTo(float px, float py);
	void MoveTo(Vector2 vec);

	// retorna tipo do objeto
	virtual uint Type() const;

	// faz a resolu��o de colis�o do objeto
	virtual void OnCollision(Object * obj);
	virtual void OnCollisionEnter(Object * obj);
	virtual void OnCollisionStay(Object * obj);
	virtual void OnCollisionExit(Object * obj);

	// ------------------------
	// fun��es virtuais puras	
	// ------------------------	 
	// devem ser obrigatoriamente sobrescritas na classe derivada

	// atualiza estado do objeto
	virtual void Update(float gameTime) = 0;

	virtual void FixedUpdate();

	// desenha o objeto na tela
	virtual void Draw() = 0;
};

// -----------------------------------------------------------------------------
// M�todos Inline

inline void Object::X(float value)
{
	position->X(value);
}

inline float Object::X() const
{
	return position->X();
}

inline void Object::Y(float value)
{
	position->Y(value);
}

inline float Object::Y() const
{
	return position->Y();
}

inline float Object::Depth() const
{
	return position->Depth();
}

inline void Object::Translate(float dx, float dy)
{
	position->Translate(dx, dy);
}

inline void Object::Translate(Vector2 vec)
{
	position->Translate(vec);
}

inline void Object::MoveTo(float px, float py)
{
	position->MoveTo(px, py);
}

inline void Object::MoveTo(Vector2 vec)
{
	position->MoveTo(vec);
}

// retorna tipo do objeto
inline uint Object::Type() const
{ return type; }

// -----------------------------------------------------------------------------

#endif


