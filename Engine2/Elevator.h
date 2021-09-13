#pragma once

#include "Component.h"
#include "Vector2.h"

class Rigidbody;
class Collision;
class Player;

class Elevator : public Component
{
private:
	Rigidbody * rigidBody;
	Collision * collision;
	Player * player;

	float pointA;
	float pointB;

	bool bActivate;
	Vector2 storedVector;

public:
	~Elevator();

	bool Init();
	void PostInit();

	void OnCollisionEnter(Object * obj);
	void OnCollisionStay(Object * obj);
	void OnCollisionExit(Object * obj);

	bool Autoplay() const;
	void Autoplay(bool value);

	void FixedUpdate();
	void Update(float gameTime);
	void Draw();

};