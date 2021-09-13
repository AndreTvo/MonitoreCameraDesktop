#pragma once

#include "Component.h"
#include "Vector2.h"
#include "Geometry.h"
#include <unordered_set>
using std::unordered_set;

class Rigidbody;
class Position;
class Object;

typedef unordered_set<Object *> HashCollision;

class Collision : public Component
{
private:
	Vector2 * vector;
	Position * position;
	Rigidbody * rigidbody;

	bool colliding;
	Object * parent;
	HashCollision collisorTable;

	bool trigger;

	/* Debug variables */
	bool debug;
	uint color;

	Geometry * bbox;

public:
	~Collision();

	bool Init();
	void PostInit();
	void Update(float gameTime);
	void Draw();

	void IsColliding(bool value);
	bool IsColliding() const;

	void BoundingBox(Geometry * geometry);
	Geometry * BoundingBox();

	void TriggerMode(bool value);
	bool TriggerMode() const;

	void Debug(bool value);
	bool Debug() const;
	void DebugCollision();

	Object * Parent();
	void Parent(Object * obj);

	uint Color() const;

	HashCollision * CollisorTable();
	void UpdateCollisionFlag();

	Rigidbody * Body();

	Vector2 * Vector();
	
};