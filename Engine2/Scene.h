/**********************************************************************************
// Scene (Arquivo de Cabe�alho)
// 
// Cria��o:		16 Mar 2012
// Atualiza��o:	16 Jul 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Define uma classe para gerenciar o cen�rio do jogo.
//				
//				Um gerenciador de cena � respons�vel por guardar os objetos 
//				da cena, atualizando-os e desenhando-os de forma mais pr�tica. 
//              Ele pode ser usado tamb�m para outras tarefas que impliquem em
//              percorrer a lista de objetos, como detec��o de colis�o.
//
**********************************************************************************/

#pragma once

// ---------------------------------------------------------------------------------
// Inclus�es

#include "Geometry.h"							// bounding boxes dos objetos
#include "Container.h"
#include <list>									// lista da biblioteca STL 
#include <vector>
#include <unordered_set>						// tabela de dispers�o
using std::list;								// acessa list sem precisar do std::
using std::vector;
using std::pair;								// acessa pair sem precisar do std:
using std::unordered_set;						// acesssa unorndered_map sem std::
class Object;									// declara��o avan�ada

// ---------------------------------------------------------------------------------

typedef pair<Object*, Object*>  ObjectPair;
typedef pair<Collision*, Collision*>  CollisionPair;

typedef pair<ObjectPair, CollisionPair>  ResolutionPair;
typedef pair<Object*, int>      ObjectDel;

/* Rectangle detection collision */
enum RectangleSide
{
	TOP,
	LEFT,
	RIGHT,
	DOWN
};

// ---------------------------------------------------------------------------------

class Scene
{
private:
	list<Object*>			staticObjects;		// lista de objetos est�ticos (n�o colidem entre si)
	list<Object*>			movingObjects;		// lista de objetos em movimento (podem colidir entre si e com os est�ticos)
	list<ResolutionPair>	collisions;			// lista de pares de objetos em colis�o
	list<ObjectDel>			toDelete;			// lista de objetos a deletar da cena

	unordered_set<Object*> resolutions;			// objetos registrados para a resolu��o de colis�o

	list<Object*>::iterator its;				// iterador para elemento est�tico
	list<Object*>::iterator itm;				// iterador para elemento em movimento
	list<Object*>::iterator it;					// iterador para elemento atual

	int processingElements;						// indica qual lista est� sendo processada

	float physicsFrameTime;
	float physicsDeltaTime;						// tempo para FixedUpdate

	bool Collision(Point* p, Rect* r);			// colis�o entre ponto e ret�ngulo	
	bool Collision(Point* p, Circle* c);		// colis�o entre ponto e c�rculo
	bool Collision(Point* p, Poly* pol);		// colis�o entre ponto e pol�gono

	bool Collision(Rect* ra, Rect* rb, Vector2 * outVector);			// colis�o entre ret�ngulos
	bool Collision(Rect* r, Point* p);			// colis�o entre ret�ngulo e ponto (inline)
	bool Collision(Rect* r, Circle* c, Vector2 * outVector);			// colis�o entre ret�ngulo e c�rculo
	bool Collision(Rect* r, Poly* pol);			// colis�o entre ret�ngulo e pol�gono

	bool Collision(Circle* ca, Circle* cb);		// colis�o entre c�rculos
	bool Collision(Circle* c, Point* p);		// colis�o entre c�rculo e ponto (inline)
	bool Collision(Circle* c, Rect* r, Vector2 * outVector);			// colis�o entre c�rculo e ret�ngulo (inline)
	bool Collision(Circle* c, Poly* pol);		// colis�o entre c�rculo e pol�gono

	bool Collision(Poly* pa, Poly* pb);			// colis�o entre pol�gonos
	bool Collision(Poly* pol, Point* p);		// colis�o entre pol�gono e ponto (inline)
	bool Collision(Poly* pol, Rect* r);			// colis�o entre pol�gono e ret�ngulo (inline)
	bool Collision(Poly* pol, Circle* c);		// colis�o entre pol�gono e c�rculo (inline)

	bool Collision(Multi* m, Geometry* s);		// colis�o entre multi e uma forma qualquer
	bool Collision(Geometry* s, Multi* m);		// colis�o entre uma forma qualquer e multi (inline)

	/* Rectangle detection collision */
	bool RectangleCollision(Rect * rectA, Rect * rectB, RectangleSide side, float * outValue);
	
public:
	Scene();									// construtor
	~Scene();									// destrutor da cena

	void Add(Object* obj, int type);			// adiciona objeto na cena (tipo STATIC ou MOVING)
	void Remove(Object* obj, int type);			// adiciona objeto a lista de elementos a remover
	void Remove();								// remove objeto sendo processado na cena
	void ProcessDeleted();						// apaga elementos marcados para exclus�o
	uint Size();								// retorna quantidade de objetos na cena

	void Begin();								// inicia percurso na lista de objetos
	Object * Next();							// retorna pr�ximo objeto da lista

	bool Collision(Geometry * bbox_a, Geometry * bbox_b, Vector2 * outVector);	// verifica se h� colis�o entre dois objetos

	void Update(float gameTime);				// atualiza todos os objetos da cena
	void Draw();								// desenha todos os objetos da cena
	void CollisionDetection();	// trata a colis�o entre objetos da cena

	void AddCollisionResolution(Object* obj);		// adiciona objeto interessado em tratar colis�o
	void RemoveCollisionResolution(Object* obj);	// remove objeto interessado em tratar colis�o
};

// --------------------------------------------------------------------------------
// Fun��es Membro Inline

// retorna quantidade de objetos na cena
inline uint Scene::Size()
{ return staticObjects.size() + movingObjects.size(); }

// colis�o entre ret�ngulo e ponto
inline bool Scene::Collision(Rect* r, Point* p)
{ return Collision(p, r); }

// colis�o entre c�rculo e ponto
inline bool Scene::Collision(Circle* c, Point* p)
{ return Collision(p, c); }

// colis�o entre c�rculo e ret�ngulo
inline bool Scene::Collision(Circle* c, Rect* r, Vector2 * outVector)
{ return Collision(r, c, outVector); }

// colis�o entre pol�gono e ponto
inline bool Scene::Collision(Poly* pol, Point* p)
{ return Collision(p, pol); }

// colis�o entre pol�gono e ret�ngulo
inline bool Scene::Collision(Poly* pol, Rect* r)
{ return Collision(r, pol); }

// colis�o entre pol�gono e c�rculo
inline bool Scene::Collision(Poly* pol, Circle* c)
{ return Collision(c, pol); }

// colis�o entre multi e uma forma qualquer
inline bool Scene::Collision(Geometry* s, Multi* m)
{ return Collision(m, s); }

// ---------------------------------------------------------------------------------