#pragma once

#include "Object.h"
#include "Timer.h"
#include "Missile.h"

enum PlayerAnimation
{
	STILL_RIGHT,
	STILL_LEFT,

	SITTING_RIGHT,
	SITTING_LEFT,

	SAT_RIGHT,
	SAT_LEFT,

	SIT_STILL_RIGHT,
	SIT_STILL_LEFT,

	JUMP_RIGHT,
	JUMP_LEFT,

	FALLING_RIGHT,
	FALLING_LEFT,

	GROUNDING_RIGHT,
	GROUNDING_LEFT,

	WALKING_RIGHT,
	WALKING_LEFT
};

class Floor;

class Player : public Object
{
private:
	PlatformPlayer * platformScript;
	Floor * passFloor;
	Missile * missile;
	string name;

	Collision * collision;
	Rigidbody * rigidbody;
	Animation * animation;

	int collisionState;

	float checkPointX;
	float checkPointY;

	bool bPrepRight;
	bool bWalkingRight;

	bool bPrepLeft;
	bool bWalkingLeft;

	bool bJumpAnim;
	bool bFallAnim;

	bool bSitting;
	bool bSittingAnim;
	bool bSittingPersistAnim;
	bool bGettingUpAnim;

	bool bFacingRight;
	bool bFacingLeft;

	bool bFacingAnim;

	bool bGroudingAnim;
	bool bCanFaceAnim;
	bool bDescendingPlatform;
	bool bCanPassThroughPlatform;
	bool bPassedThroughPlatform;

	/* One way variables */
	bool bForceCorrection;
	bool bDownUpPass;			// Pulando por baixo de uma plataforma
	bool bDownUpAir;
	bool bUpFirstDown;				// Pulando do lado da plataforma para ela
	bool bDescending;			// Descendo pela plataforma
	bool bPressJump;

	bool canJump;
	bool jumping;
	Timer timerJumping;

public:
	Player();
	~Player();

	void Update(float gameTime);
	void Draw();

	const string Name() const;
	const string CurrentAnimation() const;

	void SetCheckpoint(float x, float y);

	int CheckpointX() const;
	int CheckpointY() const;

	void OnCollision(Object * obj);
	void OnCollisionEnter(Object * obj);
	void OnCollisionStay(Object * obj);
	void OnCollisionExit(Object * obj);

	void ResetVelocity();
};