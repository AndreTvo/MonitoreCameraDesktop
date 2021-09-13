/**********************************************************************************
// Object (C�digo Fonte)
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

#include "Object.h"
#include "Engine.h"

// -------------------------------------------------------------------------------

Object::Object()
{
	container = new Container(this);
	position = container->AddComponent<Position>();

	//tipo do objeto
	type = 0;
}

// -------------------------------------------------------------------------------

Object::~Object()
{
	delete container;
}

Container * Object::Components() const
{
	return this->container;
}

// -------------------------------------------------------------------------------

void Object::OnCollision(Object * obj)
{
}

void Object::OnCollisionEnter(Object * obj)
{
}

// -------------------------------------------------------------------------------

void Object::OnCollisionStay(Object * obj)
{
}

// -------------------------------------------------------------------------------

void Object::OnCollisionExit(Object * obj)
{
}

void Object::FixedUpdate()
{
}

// -------------------------------------------------------------------------------