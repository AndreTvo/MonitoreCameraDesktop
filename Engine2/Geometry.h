/**********************************************************************************
// Geometry (Arquivo de Cabe�alho)
//
// Cria��o:		05 Oct 2007
// Atualiza��o:	16 Jul 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Agrupa a defini��o de todas as formas geom�tricas suportadas:
//              ponto, linha, ret�ngulo, c�rculo, pol�gono e multi (agrupamento
//              de uma ou mais geometrias)
//
**********************************************************************************/

#ifndef _DESENVJOGOS_GEOMETRY_H_
#define _DESENVJOGOS_GEOMETRY_H_

// ---------------------------------------------------------------------------
// Inclus�es

#include "Types.h"		// tipos da engine
#include "Vector2.h"	// vetor no r2
#include <list>			// lista da STL
#include <unordered_map>// tabela de dispers�o
using std::list;		// usa list sem std::

class Collision;

// ---------------------------------------------------------------------------
// Geometry
// ---------------------------------------------------------------------------

/* TODO:
 *
 * Remanejar vetor de posi��o com a geometria
 *
 */

class Geometry
{
protected:
	uint type;											// tipo da geometria
	Vector2 vector;										// posi��o da geometria

public:
	Geometry();											// construtor
	virtual ~Geometry();								// destrutor

	uint Type() const { return type; };

	virtual float X() const { return vector.X(); }		// coordenada x da geometria
	virtual float Y() const { return vector.Y(); }		// coordenada y da geometria
		
	virtual void Translate(float dx, float dy)			// move a geometria pelo delta (dx,dy)
	{ 
		vector.Translate(dx, dy); 
	}

	virtual void Translate(const Vector2 & vect)		// move a geometria pelo vetor
	{ 
		vector.Translate(vect);
	}

	virtual void MoveTo(float px, float py)				// move a geometria para a posi��o (px,py)
	{ vector.MoveTo(px, py); }

	virtual void MoveTo(const Vector2 & vect)				// move a geometria para a posi��o (px,py)
	{ vector.MoveTo(vect); }

	virtual void Rotate(float angle) = 0;				// rotaciona uma geometria
};

// --------------------------------------------------------------------------
// Point
// --------------------------------------------------------------------------

class Point : public Geometry
{
public:

	Point();											// construtor padr�o
	Point(float posX, float posY);						// construtor usando pontos-flutuantes
	Point(int posX, int posY);							// construtor usando inteiros
	
	void Rotate(float angle) {};						// rociona o ponto pelo �ngulo dado
	
	float Distance(Point & p) const;					// calcula a dist�ncia at� outro ponto
};

// --------------------------------------------------------------------------
// Line
// -------------------------------------------------------------------------

class Line : public Geometry
{
public:
	Point a, b;											// linha vai do ponto A ao ponto B
	
	Line();												// construtor padr�o
	Line(float x1, float y1, float x2, float y2);		// construtor usando pontos-flutuantes
	Line(Point& pa, Point& pb);							// construtor usando pontos

	void Rotate(float angle);							// rociona uma linha pelo �ngulo dado

	float Ax() const { return vector.X() + a.X(); }		// coordenadas do mundo do ponto A eixo x
	float Ay() const { return vector.Y() + a.Y(); }		// coordenadas do mundo do ponto A eixo y
	float Bx() const { return vector.X() + b.X(); }		// coordenadas do mundo do ponto B eixo x
	float By() const { return vector.Y() + b.Y(); }		// coordenadas do mundo do ponto B eixo y
};

// --------------------------------------------------------------------------
// Rect
// --------------------------------------------------------------------------

class Rect : public Geometry
{
public:
	float left;											// coordenada esquerda do ret�ngulo
	float top;											// coordenada superior do ret�ngulo
	float right;										// coordenada direita do ret�ngulo
	float bottom;										// coordenada inferior do ret�ngulo

	Rect();												// construtor padr�o
	Rect(float height, float width);
	Rect(Point& a, Point& b);							// construtor usando pontos

	void  Rotate(float angle) {};						// rotaciona o ret�ngulo pelo �ngulo dado

	void Height(float height) {
		top = -height / 2;
		bottom = height / 2;
	}

	float Height() const { return Bottom() - Top(); }

	void Width(float width) {
		right = width / 2;
		left = -width / 2;
	}

	float Width() const  { return Right() - Left(); }

	float Left() const   { return vector.X() + left; }	// coordenadas do mundo do menor valor do eixo x
	float Top() const    { return vector.Y() + top; }	// coordenadas do mundo do menor valor do eixo y
	float Right() const  { return vector.X() + right; }	// coordenadas do mundo do maior valor do eixo x
	float Bottom() const { return vector.Y() + bottom; }// coordenadas do mundo do maior valor do eixo y
};

// --------------------------------------------------------------------------
// Circle
// --------------------------------------------------------------------------

class Circle : public Geometry
{
public:
	float radius;										// raio do c�rculo

	void  Rotate(float angle) {};						// rotaciona o c�rculo pelo �ngulo dado

	Circle();											// construtor padr�o
	Circle(float r);									// contrutor com raio
};

// --------------------------------------------------------------------------
// Poly
// --------------------------------------------------------------------------

class Poly : public Geometry
{
private:
	Circle *  bbox;										// bounding box do pol�gono
	void      BoundingBox();							// calcula a bounding box do pol�gono

public:
	uint      vertexCount;								// n�mero de v�rtices 
	Point*    vertexList;								// vetor de v�rtices do pol�gono

	Poly();												// construtor padr�o
	Poly(Point * vList, uint vCount);					// construtor
	Poly(const Poly& p);								// construtor de c�pia
	~Poly();											// destructor

	// necess�rio para tratar o ponteiro na atribui��o
	const Poly& operator=(const Poly& p);				// operador de atribui��o

	void Translate(float dx, float dy);					// move pol�gono pelo delta (dx,dy)
	void MoveTo(float px, float py);					// move pl�gono para a posi��o (px,py)
	void Rotate(float angle);							// rotaciona pol�gono
	
	void Vertices(const Point * vList, uint vCount);	// muda lista de v�rtices do pol�gono
	Circle* BBox() { return bbox; }						// retorna bounding box do pol�gono
}; 

// --------------------------------------------------------------------------
// Multi
// --------------------------------------------------------------------------

class Multi : public Geometry
{
public:
	list<Geometry*> shapes;								// lista de formas geom�tricas
	
	Multi();											// construtor padr�o
	~Multi();											// destructor

	void Insert(Geometry * s);							// insere geometria na lista
	void Remove(Geometry * s);							// remove geometria da lista

	void Translate(float dx, float dy);					// move a geometria pelo delta (dx,dy)
	void MoveTo(float px, float py);					// move a geometria para a posi��o (px,py)
};

// --------------------------------------------------------------------------

#endif
