/**********************************************************************************
// Object (Código Fonte)
//
// Criação:		01 Out 2007
// Atualização:	14 Mai 2014
// Compilador:	Visual C++ 12
//
// Descrição:	Essa é a classe base para todos objetos do jogo.
//
//				Um objeto do jogo é qualquer coisa que possámos querer desenhar
//				ou interagir dentro do jogo. Por exemplo, um soldado, um prédio,
//				um projétil, uma forma geométrica, etc.
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