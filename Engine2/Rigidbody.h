#pragma once

#include "Component.h"
#include "Vector2.h"
#include "Timer.h"

class PlatformPlayer;

class Rigidbody : public Component
{
private:
	Vector2 gravity;
	Vector2 acceleration;
	Vector2 velocity;

	Object * parent;

	float lossEnergy;
	float maxSpeed;

	float physicsFrameTime;

public:
	~Rigidbody();

	bool Init();
	void PostInit();

	void FixedUpdate();
	void Update(float gameTime);
	void Draw();

	Object * Parent() const;
	void Parent(Object * obj);

	void Gravity(Vector2 vect);
	Vector2 Gravity() const;
	Vector2 Velocity() const;

	void AddForce(const Vector2 & vect);

	void Bounciness(float value);
	float Bounciness() const;

	void MaxSpeedX(float speed);

	void ResetVelocity();
	void ResetXVelocity();
	void ResetYVelocity();

	void ResetAcceleration();
	void ResetXAcceleration();
	void ResetYAcceleration();
};
