#pragma once

#include "Component.h"
#include "Collision.h"

class Rigidbody;

class PlatformPlayer : public Component
{
private:
	Rect * bboxPlayer;
	Rect bboxTop, bboxBottom;

	Vector2 acceleration;

	Rigidbody * rigidbodyPlayer;
	Collision * collisionPlayer;
	Object * groundObject;

	bool bGravity;
	bool bNoPlatform;
	bool bGrounded;

public:
	~PlatformPlayer();

	bool Init();
	void PostInit();

	void Update(float gameTime);
	void Draw();

	void Gravity(bool value);
	bool Gravity() const;

	void UpdateGroundFlag();
	void UpdateCollisionFlag();

	Collision * CollisionPlayer();
	Object * GroundObject() const;
	bool GroundCollision(Object * obj);
	bool PerfectGroundCollision(Object * obj);
	void SaveAccelerationVector(Vector2 vector);
	void ForceNotGrounded();

	bool IsGrounded() const;
};