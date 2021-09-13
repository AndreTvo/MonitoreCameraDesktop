/**********************************************************************************
// Scene (C�digo Fonte)
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

#include "Scene.h"
#include "Object.h"

// ---------------------------------------------------------------------------------

Scene::Scene()
{
	processingElements = STATIC;

	physicsDeltaTime = 0;
	physicsFrameTime = Engine::PhysicsFrameTime();
}

// ---------------------------------------------------------------------------------

Scene::~Scene()
{
	// remove da mem�ria os objetos est�ticos
	for (auto i = staticObjects.begin(); i != staticObjects.end(); ++i)
		delete *i;

	// remove da mem�ria os objetos em movimento
	for (auto i = movingObjects.begin(); i != movingObjects.end(); ++i)
		delete *i;

	// limpa listas
	staticObjects.clear();
	movingObjects.clear();
}

// ---------------------------------------------------------------------------------

void Scene::Add(Object * obj, int type)
{
	// insere novo objeto na cena
	if (type == STATIC)
		staticObjects.push_back(obj);
	else
		movingObjects.push_back(obj);
}


// ---------------------------------------------------------------------------------

void Scene::Remove(Object* obj, int type)
{
	toDelete.push_back(ObjectDel(obj, type));
}

// ---------------------------------------------------------------------------------

void Scene::Remove()
{
	toDelete.push_back(ObjectDel(*it, processingElements));
}

// ---------------------------------------------------------------------------------

void Scene::ProcessDeleted()
{
	// remove objetos repetidos
	toDelete.sort();
	toDelete.unique();

	for (auto i = toDelete.begin(); i != toDelete.end(); ++i)
	{
		for (auto j : i->first->Components()->Data())
			delete j.second;

		// libera mem�ria ocupada pelo objeto
 		delete i->first;

		// remove objeto da cena
		if (i->second == STATIC)
			staticObjects.remove(i->first);
		else
			movingObjects.remove(i->first);

		// remove objeto da lista de objetos registrados para a resolu��o de colis�o
		resolutions.erase(i->first);		
	}

	toDelete.clear();
}

// ---------------------------------------------------------------------------------

void Scene::Update(float gameTime)
{
	physicsDeltaTime += gameTime;

	// atualiza todos os objetos a cada tempo fixo determinado
	if (physicsDeltaTime >= physicsFrameTime)
	{
		// atualiza todos os objetos est�ticos
		processingElements = STATIC;
		for (it = staticObjects.begin(); it != staticObjects.end(); ++it)
		{
			for (auto component : (*it)->Components()->Data())
			{
				if (component.second->Enabled())
					component.second->FixedUpdate();
			}

			(*it)->FixedUpdate();
		}

		// atualiza todos os objetos em movimento
		processingElements = MOVING;
		for (it = movingObjects.begin(); it != movingObjects.end(); ++it)
		{
			for (auto component : (*it)->Components()->Data())
			{
				if (component.second->Enabled())
					component.second->FixedUpdate();
			}

			(*it)->FixedUpdate();
		}

		physicsDeltaTime -= physicsFrameTime;
	}

	// atualiza todos os objetos est�ticos
	processingElements = STATIC;
	for (it = staticObjects.begin(); it != staticObjects.end(); ++it)
	{
		for (auto component : (*it)->Components()->Data())
		{
			if (component.second->Enabled())
				component.second->Update(gameTime);
		}

		(*it)->Update(gameTime);
	}

	// atualiza todos os objetos em movimento
	processingElements = MOVING;
	for (it = movingObjects.begin(); it != movingObjects.end(); ++it)
	{
		for (auto component : (*it)->Components()->Data())
		{
			if (component.second->Enabled())
				component.second->Update(gameTime);
		}

		(*it)->Update(gameTime);
	}


	ProcessDeleted();
	CollisionDetection();
}

// ---------------------------------------------------------------------------------

void Scene::Draw()
{
	// desenha todos os objetos est�ticos
	processingElements = STATIC;
	for (it = staticObjects.begin(); it != staticObjects.end(); ++it)
	{
		for (auto component : (*it)->Components()->Data())
		{
			if (component.second->Enabled())
				component.second->Draw();
		}

		(*it)->Draw();
	}
	

	// desenha todos os objetos em movimento
	processingElements = MOVING;
	for (it = movingObjects.begin(); it != movingObjects.end(); ++it)
	{
		for (auto component : (*it)->Components()->Data())
		{
			if (component.second->Enabled())
				component.second->Draw();
		}

		(*it)->Draw();
	}
	
}

// ---------------------------------------------------------------------------------

void Scene::Begin()
{
	// aponta para o primeiro elemento de cada lista
	its = staticObjects.begin();
	itm = movingObjects.begin();
	processingElements = STATIC;
}

// ---------------------------------------------------------------------------------

Object * Scene::Next()
{
	// se apontador aponta para objeto v�lido
	if (its != staticObjects.end())
	{
		// passa ao pr�ximo objeto
		// guarda apontador para o anterior
		it = its++;
		return *it;
	}
	else
	if (itm != movingObjects.end())
	{
		processingElements = MOVING;
		it = itm++;
		return *it;
	}
	else
		// chegou ao fim das listas
		return nullptr;
}

bool Scene::Collision(Geometry * bbox_a, Geometry * bbox_b, Vector2 * outVector)
{
	// um dos objetos n�o tem bounding box
	if (!(bbox_a
		&& bbox_b))
		return false;

	uint type_a = bbox_a->Type();
	uint type_b = bbox_b->Type();

	bool ret = false;

	switch (type_a)
	{
		// Point	
	case POINT_T:

		switch (type_b)
		{
		case CIRCLE_T:
			// Point && Circle
			ret = Collision((Point*)bbox_a, (Circle*)bbox_b);
			break;

		case RECTANGLE_T:
			// Point && Rectangle
			ret = Collision((Point*)bbox_a, (Rect*)bbox_b);
			break;

		case POLYGON_T:
			// Point && Polygon
			ret = Collision((Point*)bbox_a, (Poly*)bbox_b);
			break;

		case MULTI_T:
			// Point && Multi
			ret = Collision(bbox_a, (Multi*)bbox_b);
			break;

		default:
			// Point && Unknown
			ret = false;
		}

		break;

		// Circle	
	case CIRCLE_T:

		switch (type_b)
		{
		case POINT_T:
			// Circle && Point
			ret = Collision((Circle*)bbox_a, (Point*)bbox_b);
			break;

		case CIRCLE_T:
			// Circle && Circle
			ret = Collision((Circle*)bbox_a, (Circle*)bbox_b);
			break;

		case RECTANGLE_T:
			// Circle && Rectangle
			ret = Collision((Circle*)bbox_a, (Rect*)bbox_b, outVector);
			break;

		case POLYGON_T:
			// Circle && Polygon
			ret = Collision((Circle*)bbox_a, (Poly*)bbox_b);
			break;

		case MULTI_T:
			// Circle && Multi
			ret = Collision(bbox_a, (Multi*)bbox_b);
			break;

		default:
			// Circle && Unknown
			ret = false;
			break;
		}

		break;

		// Rectangle
	case RECTANGLE_T:

		switch (type_b)
		{
		case POINT_T:
			// Rectangle && Point
			ret = Collision((Rect*)bbox_a, (Point*)bbox_b);
			break;

		case CIRCLE_T:
			// Rectangle && Circle
			ret = Collision((Rect*)bbox_a, (Circle*)bbox_b, outVector);
			break;

		case RECTANGLE_T:

			// Rectangle && Rectangle
			ret = Collision((Rect*)bbox_a, (Rect*)bbox_b, outVector);
			break;

		case POLYGON_T:
			// Rectangle && Polygon
			ret = Collision((Rect*)bbox_a, (Poly*)bbox_b);
			break;

		case MULTI_T:
			// Rectangle && Multi
			ret = Collision(bbox_a, (Multi*)bbox_b);
			break;

		default:
			// Rectangle && Unknown
			ret = false;
			break;
		}

		break;

		// Polygon
	case POLYGON_T:

		switch (type_b)
		{
		case POINT_T:
			// Polygon && Point
			ret = Collision((Poly*)bbox_a, (Point*)bbox_b);
			break;

		case CIRCLE_T:
			// Polygon && Circle
			ret = Collision((Poly*)bbox_a, (Circle*)bbox_b);
			break;

		case RECTANGLE_T:
			// Polygon && Rectangle
			ret = Collision((Poly*)bbox_a, (Rect*)bbox_b);
			break;

		case POLYGON_T:
			// Polygon && Polygon
			ret = Collision((Poly*)bbox_a, (Poly*)bbox_b);
			break;

		case MULTI_T:
			// Polygon && Multi
			ret = Collision(bbox_a, (Multi*)bbox_b);
			break;

		default:
			// Polygon && Unknown
			ret = false;
		}

		break;

		// Multi
	case MULTI_T:
		// Multi && Geometry
		ret = Collision((Multi*)bbox_a, bbox_b);
		break;


		// Unknown
	default:
		ret = false;
		break;
	}

	return ret;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Rect * r)
{
	// se as coordenadas  do ponto est�o dentro do ret�ngulo
	if (p->X() >= r->Left() && p->X() <= r->Right())
		if (p->Y() >= r->Top() && p->Y() <= r->Bottom())
			return true;

	// caso contr�rio n�o h� colis�o
	return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Circle * c)
{
	// se a dist�ncia entre o ponto e o centro do c�rculo
	// for menor que o raio do c�rculo ent�o h� colis�o
	if (p->Distance(Point(c->X(), c->Y())) <= c->radius)
		return true;
	else
		return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Poly * pol)
{
	// se o ponto colidir com a bounding box do pol�gono,
	// passe para uma investiga��o mais profunda e lenta
	// caso contr�rio n�o h� colis�o

	if (Collision(p, pol->BBox()))
	{
		bool crossings = false;
		int nvertex = pol->vertexCount;
		Point * vertex = pol->vertexList;

		float Ix, Iy, Jx, Jy;

		for (int i = 0, j = nvertex - 1; i < nvertex; j = i++)
		{
			// transforma coordenadas locais em globais
			Ix = vertex[i].X() + pol->X();
			Iy = vertex[i].Y() + pol->Y();
			Jx = vertex[j].X() + pol->X();
			Jy = vertex[j].Y() + pol->Y();

			if (((Iy < p->Y()) && (Jy >= p->Y())) || ((Jy < p->Y()) && (Iy >= p->Y())))
				if (Jx - (((Jy - p->Y())*(Jx - Ix)) / (Jy - Iy)) < p->X())
					crossings = !crossings;
		}

		return crossings;
	}

	return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * ra, Rect * rb, Vector2 * outVector)
{
	// verificando sobreposi��o no eixo x
	bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right());

	// verificando sobreposi��o no eixo y
	bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom());

	// existe colis�o se h� sobreposi��o nos dois eixos
	return overlapX && overlapY;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Circle * c, Vector2 * outVector)
{
	// encontra o ponto do ret�ngulo mais pr�ximo do centro do c�rculo
	float px, py;

	int xDirection = 0;
	int yDirection = 0;

	// eixo x
	if (c->X() < r->Left())
	{
		px = r->Left();
		xDirection = -1;
	}
	else
	{
		if (c->X() > r->Right())
		{
			px = r->Right();
			xDirection = 1;
		}
		else
			px = c->X();
	}

	// eixo y
	if (c->Y() < r->Top())
	{
		py = r->Top();
		yDirection = -1;
	}
	else
	{
		if (c->Y() > r->Bottom())
		{
			py = r->Bottom();
			yDirection = +1;
		}
		else
			py = c->Y();
	}

	// verifica se h� colis�o entre este ponto e o c�rculo
	Point point(px, py);
	bool res = Collision(&point, c);

	if (res)
	{
		float xDist = abs(px - c->X());
		float yDist = abs(py - c->Y());

		float xPen = c->radius - xDist;
		float yPen = c->radius - yDist;

		if (outVector)
			*outVector = Vector2((xPen - yDist) * xDirection, (yPen - xDist) * yDirection);
	}
	
	return  res;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Poly * pol)
{
	// se o ret�ngulo colidir com a bounding box do pol�gono,
	// passe para uma investiga��o mais profunda e lenta
	// caso contr�rio n�o h� colis�o

	if (Collision(r, pol->BBox(), nullptr))
	{
		// recupera os cantos do ret�ngulo
		Point corners[4] =
		{
			Point(r->Left(), r->Top()),
			Point(r->Right(), r->Top()),
			Point(r->Right(), r->Bottom()),
			Point(r->Left(), r->Bottom())
		};

		// verifica se algum canto do ret�ngulo est� dentro do pol�gono
		for (int i = 0; i < 4; ++i)
			if (Collision(&corners[i], pol))
				return true;

		// recupera v�rtices do pol�gono
		int nv = pol->vertexCount;
		Point * v = pol->vertexList;

		// verifica se algum v�rtice est� dentro do ret�ngulo
		float pX, pY;
		for (int i = 0; i < nv; ++i)
		{
			// transforma coordenadas locais em globais
			pX = v[i].X() + pol->X();
			pY = v[i].Y() + pol->Y();

			Point p(pX, pY);

			if (Collision(&p, r))
				return true;
		}
	}

	return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle * ca, Circle * cb)
{
	// soma dos raios
	float sumR = ca->radius + cb->radius;
	sumR *= sumR;

	bool res = (sumR < pow((ca->X() + cb->X()), 2) + pow((ca->Y() + cb->Y()), 2));

	// nenhum colis�o detectada
	return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle * c, Poly * pol)
{
	// se o c�rculo colidir com a bounding box do pol�gono,
	// passe para uma investiga��o mais profunda e lenta
	// caso contr�rio n�o h� colis�o

	if (Collision(c, pol->BBox()))
	{
		// recupera v�rtices do pol�gono
		int nv = pol->vertexCount;
		Point * v = pol->vertexList;

		// TODO: identificar o ponto do c�rculo mais pr�ximo de cada aresta 
		// e verificar se este ponto est� dentro do pol�gono

		// verifica se v�rtices est�o dentro do c�rculo
		float pX, pY;
		for (int i = 0; i < nv; ++i)
		{
			// transforma coordenadas locais em globais
			pX = v[i].X() + pol->X();
			pY = v[i].Y() + pol->Y();

			Point p(pX, pY);

			if (Collision(&p, c))
				return true;
		}
	}

	return false;

}

// --------------------------------------------------------------------------------

bool Scene::Collision(Poly* pa, Poly* pb)
{
	// se as bounding boxes estiverem colidindo, 
	// passe para uma investiga��o mais profunda e lenta
	// caso contr�rio n�o h� colis�o

	if (Collision(pa->BBox(), pb->BBox()))
	{
		// recupera v�rtices do pol�gono A
		int nva = pa->vertexCount;
		Point * va = pa->vertexList;

		float pX, pY;

		// verifica se v�rtices de A est�o dentro do pol�gono B
		for (int i = 0; i < nva; ++i)
		{
			// transforma coordenadas locais em globais
			pX = va[i].X() + pa->X();
			pY = va[i].Y() + pa->Y();
			
			Point p(pX, pY);

			if (Collision(&p, pb))
				return true;
		}

		// recupera v�rtices do pol�gono B
		int nvb = pb->vertexCount;
		Point * vb = pb->vertexList;

		// verifica se v�rtices de B est�o dentro do pol�gono A
		for (int i = 0; i < nvb; ++i)
		{
			// transforma coordenadas locais em globais
			pX = vb[i].X() + pb->X();
			pY = vb[i].Y() + pb->Y();
			
			Point p(pX, pY);

			if (Collision(&p, pa))
				return true;
		}
	}

	return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Multi* m, Geometry* s)
{
	bool collision = false;

	// percorra lista at� achar uma colis�o
	auto i = m->shapes.begin();

	while (!collision && i != m->shapes.end())
	{
		switch ((*i)->Type())
		{
			// Point
		case POINT_T:
			switch (s->Type())
			{
			case CIRCLE_T:    collision = Collision((Point*)(*i), (Circle*)s); break;
			case RECTANGLE_T: collision = Collision((Point*)(*i), (Rect*)s);   break;
			case POLYGON_T:   collision = Collision((Point*)(*i), (Poly*)s);   break;
			case MULTI_T:     collision = Collision((Point*)(*i), (Multi*)s);  break;
			}
			break;

			// Circle
		case CIRCLE_T:
			switch (s->Type())
			{
			case POINT_T:     collision = Collision((Circle*)(*i), (Point*)s);  break;
			case CIRCLE_T:    collision = Collision((Circle*)(*i), (Circle*)s); break;
			case RECTANGLE_T: collision = Collision((Circle*)(*i), (Rect*)s, nullptr);	break;
			case POLYGON_T:   collision = Collision((Circle*)(*i), (Poly*)s);   break;
			case MULTI_T:     collision = Collision((Circle*)(*i), (Multi*)s);	break;
			}
			break;

			// Rectangle
		case RECTANGLE_T:
			switch (s->Type())
			{
			case POINT_T:     collision = Collision((Rect*)(*i), (Point*)s);  break;
			case CIRCLE_T:    collision = Collision((Rect*)(*i), (Circle*)s, nullptr); break;
			case RECTANGLE_T: collision = Collision((Rect*)(*i), (Rect*)s, nullptr);	  break;
			case POLYGON_T:   collision = Collision((Rect*)(*i), (Poly*)s);   break;
			case MULTI_T:     collision = Collision((Rect*)(*i), (Multi*)s);  break;
			}
			break;

			// Polygon
		case POLYGON_T:
			switch (s->Type())
			{
			case POINT_T:     collision = Collision((Poly*)(*i), (Point*)s);  break;
			case CIRCLE_T:    collision = Collision((Poly*)(*i), (Circle*)s); break;
			case RECTANGLE_T: collision = Collision((Poly*)(*i), (Rect*)s);	  break;
			case POLYGON_T:   collision = Collision((Poly*)(*i), (Poly*)s);   break;
			case MULTI_T:     collision = Collision((Poly*)(*i), (Multi*)s);  break;
			}
			break;

			// Multi
		case MULTI_T:
			collision = Collision((Multi*)(*i), s);
			break;
		}

		// passa para a pr�xima geometria do multi
		++i;
	}

	return collision;
}

// ---------------------------------------------------------------------------------

void Scene::CollisionDetection()
{
	// iteradores para as listas
	list<Object*>::iterator start, end;

	// limpa lista de colis�es
	collisions.clear();

	// -------------------
	// Detec��o da colis�o
	// -------------------
	// se existe pelo menos dois objetos que se movem, teste a colis�o entre eles
	if (movingObjects.size() >= 2)
	{
		// pega iterador para �ltimo elemento da lista
		end = movingObjects.end();
		end--;

		// testa colis�o entre todos os pares de objetos que se movem
		for (auto i = movingObjects.begin(); i != end; ++i)
		{
			start = i;

			// j inicia no segundo elemento da lista
			for (auto j = ++start; j != movingObjects.end(); ++j)
			{
				Object * oa = *i;
				Object * ob = *j;

				::Collision * collision_a = oa->Components()->GetComponent<::Collision>();
				::Collision * collision_b = ob->Components()->GetComponent<::Collision>();

				if (collision_a && collision_b)
				{
					Geometry * geometry_a = collision_a->BoundingBox();
					Geometry * geometry_b = collision_b->BoundingBox();

					if (Collision(geometry_a, geometry_b, nullptr))
					{
						collisions.push_back(ResolutionPair(ObjectPair(oa, ob), CollisionPair(collision_a, collision_b)));
					}
					else
					{
						if (collision_a->CollisorTable()->count(ob) && collision_b->CollisorTable()->count(oa))
						{
							oa->OnCollisionExit(ob);
							for (auto component : oa->Components()->Data())
							{
								if (component.second->Enabled())
									component.second->OnCollisionExit(ob);
							}

							ob->OnCollisionExit(oa);
							for (auto component : ob->Components()->Data())
							{
								if (component.second->Enabled())
									component.second->OnCollisionExit(oa);
							}

							collision_a->CollisorTable()->erase(ob);
							collision_b->CollisorTable()->erase(oa);

							collision_a->UpdateCollisionFlag();
							collision_b->UpdateCollisionFlag();
						}
					}
				}
			}
		}
	}

	// testa colis�o entre objetos que se movem e objetos est�ticos
	for (auto i = movingObjects.begin(); i != movingObjects.end(); ++i)
	{
		for (auto j = staticObjects.begin(); j != staticObjects.end(); ++j)
		{
			Object * oa = *i;
			Object * ob = *j;

			::Collision * collision_a = oa->Components()->GetComponent<::Collision>();
			::Collision * collision_b = ob->Components()->GetComponent<::Collision>();

			if (collision_a && collision_b)
			{
				Geometry * geometry_a = collision_a->BoundingBox();
				Geometry * geometry_b = collision_b->BoundingBox();

				if (Collision(geometry_a, geometry_b, nullptr))
				{
					if (collision_a->Parent() != ob)
					{
						collisions.push_back(ResolutionPair(ObjectPair(oa, ob), CollisionPair(collision_a, collision_b)));
					}
				}
				else
				{
					if (collision_a->CollisorTable()->count(ob) && collision_b->CollisorTable()->count(oa))
					{
						oa->OnCollisionExit(ob);
						for (auto component : oa->Components()->Data())
						{
							if (component.second->Enabled())
								component.second->OnCollisionExit(ob);
						}

						ob->OnCollisionExit(oa);
						for (auto component : ob->Components()->Data())
						{
							if (component.second->Enabled())
								component.second->OnCollisionExit(oa);
						}

						collision_a->CollisorTable()->erase(ob);
						collision_b->CollisorTable()->erase(oa);

						collision_a->UpdateCollisionFlag();
						collision_b->UpdateCollisionFlag();

						if (collision_a->Parent() == ob)
						{
							collision_a->Parent(nullptr);
							collision_b->Parent(nullptr);
						}
					}
				}
			}
		}
	}

	// --------------------
	// Resolu��o da colis�o
	// --------------------
	if (!collisions.empty())
	{
		for (auto i = collisions.begin(); i != collisions.end(); ++i)
		{
			Object * oa = i->first.first;
			Object * ob = i->first.second;

			::Collision * collision_a = i->second.first;
			::Collision * collision_b = i->second.second;

			oa->OnCollision(ob);
			for (auto component : oa->Components()->Data())
			{
				if (component.second->Enabled())
					component.second->OnCollision(ob);
			}

			ob->OnCollision(oa);
			for (auto component : ob->Components()->Data())
			{
				if (component.second->Enabled())
					component.second->OnCollision(oa);
			}			

			if (collision_a->CollisorTable()->count(ob) && collision_b->CollisorTable()->count(oa))
			{
				oa->OnCollisionStay(ob);
				for (auto component : oa->Components()->Data())
				{
					if (component.second->Enabled())
						component.second->OnCollisionStay(ob);
				}

				ob->OnCollisionStay(oa);
				for (auto component : ob->Components()->Data())
				{
					if (component.second->Enabled())
						component.second->OnCollisionStay(oa);
				}
			}
			else
			{
				collision_a->CollisorTable()->emplace(ob);
				collision_b->CollisorTable()->emplace(oa);

				collision_a->UpdateCollisionFlag();
				collision_b->UpdateCollisionFlag();

				collision_a->DebugCollision();
				collision_b->DebugCollision();

				oa->OnCollisionEnter(ob);
				for (auto component : oa->Components()->Data())
				{
					if (component.second->Enabled())
						component.second->OnCollisionEnter(ob);
				}

				ob->OnCollisionEnter(oa);
				for (auto component : ob->Components()->Data())
				{
					if (component.second->Enabled())
						component.second->OnCollisionEnter(oa);
				}
			}
		}
	}

	ProcessDeleted();
}

// --------------------------------------------------------------------------------

void Scene::AddCollisionResolution(Object* obj)
{
	resolutions.insert(obj);
}

void Scene::RemoveCollisionResolution(Object * obj)
{
	resolutions.erase(obj);
}

// --------------------------------------------------------------------------------