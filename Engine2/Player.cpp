#include "Player.h"
#include "Floor.h"

Player::Player()
{
	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(new Rect(28, 14));
	collision->Debug(false);

	type = 1;
	collisionState = 1;
	checkPointX = 0;
	checkPointY = 0;

	rigidbody = Components()->AddComponent<Rigidbody>();
	rigidbody->MaxSpeedX(4);

	platformScript = Components()->AddComponent<PlatformPlayer>();
	
	animation = Components()->AddComponent<Animation>();
	//animation->Depth(0.0f);

	TileSet * tileSet = new TileSet("Resources/Sheet1.png", 64, 64, 8, 64);
	animation->SetTileSet(tileSet);
	animation->Delay(0.1f);
	animation->Repeat(true);
	animation->Scale(1.0f);

	uint jumpLeftSeq[4] = { 24, 25, 26, 27 };
	uint jumpRightSeq[4] = { 28, 29, 30, 31 };

	uint fallingRightSeq[4] = { 40,41,42,43 };
	uint fallingLeftSeq[4] = { 32,33,34,35 };

	uint groundingLeftSeq[4] = { 48, 49, 50, 51};
	uint groundingRightSeq[4] = { 59, 58, 57, 56};

	uint stillRightSeq[4] = { 0, 1, 2, 1 };
	uint stillLeftSeq[4] = { 3, 4, 5, 4 };

	uint walkingRight[8] = { 8,9,10,11,12,13,14,11 };
	uint walkingLeft[8] = { 22, 21, 20, 19, 18, 17, 16, 19};

	uint sittingLeftSeq[3] = {52, 53, 54};
	uint sittingRightSeq[3] = { 62, 61, 60 };

	uint goingStillLeftSeq[3] = { 54, 53, 52 };
	uint goingStillRightSeq[3] = { 60, 61, 62 };

	uint satLeftSeq[4] = { 36, 37, 38, 37};
	uint satRightSeq[4] = { 44, 45, 46, 45 };

	animation->Add(STILL_RIGHT, stillRightSeq, 4);
	animation->Add(STILL_LEFT, stillLeftSeq, 4);

	animation->Add(JUMP_RIGHT, jumpRightSeq, 4);
	animation->Add(JUMP_LEFT, jumpLeftSeq, 4);

	animation->Add(FALLING_RIGHT, fallingRightSeq, 4);
	animation->Add(FALLING_LEFT, fallingLeftSeq, 4);

	animation->Add(GROUNDING_RIGHT, groundingRightSeq, 4);
	animation->Add(GROUNDING_LEFT, groundingLeftSeq, 4);

	animation->Add(SITTING_RIGHT, sittingRightSeq, 3);
	animation->Add(SITTING_LEFT, sittingLeftSeq, 3);

	animation->Add(SAT_RIGHT, satRightSeq, 4);
	animation->Add(SAT_LEFT, satLeftSeq, 4);

	animation->Add(SIT_STILL_RIGHT, goingStillRightSeq, 3);
	animation->Add(SIT_STILL_LEFT, goingStillLeftSeq, 3);

	animation->Add(WALKING_RIGHT, walkingRight, 8);
	animation->Add(WALKING_LEFT, walkingLeft, 8);

	animation->Select(STILL_RIGHT);
	animation->Enabled(true);

	canJump = false;

	bWalkingRight = false;
	bWalkingLeft = false;

	bSitting = false;
	bSittingAnim = false;
	bSittingPersistAnim = false;
	bGettingUpAnim = false;

	bJumpAnim = false;
	bFallAnim = false;
	bGroudingAnim = false;
	bCanFaceAnim = true;
	bDescendingPlatform = false;
	bCanPassThroughPlatform = false;

	bFacingRight = true;
	bFacingLeft = false;
}

Player::~Player()
{
}

void Player::OnCollision(Object * obj)
{
	if (collision->Parent() != obj)
	{
		if (obj->Type() == PLATFORM)
		{
			Floor * cFloor = ((Floor*)obj);
			Vector2 outVector;

			Rect * ra = (Rect *)collision->BoundingBox();
			Rect * rb = (Rect *)obj->Components()->GetComponent<Collision>()->BoundingBox();

			float lengthX = rb->X() - ra->X();

			float halfWidthRectA = ra->Width();
			float halfWidthRectB = rb->Width();

			float gapLengthX = abs(lengthX) - (halfWidthRectA + halfWidthRectB) / 2;

			if (gapLengthX <= 0)
			{
				float lengthY = rb->Y() - ra->Y();

				float halfHeightRectA = ra->Height();
				float halfHeightRectB = rb->Height();

				float gapLengthY = abs(lengthY) - (halfHeightRectA + halfHeightRectB) / 2;

				if (gapLengthY <= 0)
				{
					if (gapLengthX > gapLengthY)
					{
						if (lengthX > 0)
						{
							(outVector)[0] = gapLengthX; // Esquerda para direita
							(outVector)[1] = 0;
						}
						else
						{
							(outVector)[0] = -gapLengthX; // Direita pra esquerda
							(outVector)[1] = 0;
						}
					}
					else
					{
						if (lengthY > 0)
						{
							(outVector)[0] = 0;
							(outVector)[1] = gapLengthY; // Cima pra baixo
						}
						else
						{
							(outVector)[0] = 0;
							(outVector)[1] = -gapLengthY; // Baixo pra cima
						}
					}
				}
			}

			if ((abs(outVector.X()) > 0 || abs(outVector.Y()) > 0))
			{
				Translate(outVector);

				if (rigidbody && !cFloor->Passthrough())
				{
					if (abs(outVector.X()) > 0)
					{
						rigidbody->ResetXVelocity();
						rigidbody->ResetXAcceleration();
					}
					else if (abs(outVector.Y()) > 0)
					{
						rigidbody->ResetYVelocity();
						rigidbody->ResetYAcceleration();
					}
				}
			}
		}
	}
}

void Player::OnCollisionEnter(Object * obj)
{
	if (obj->Type() == PLATFORM)
	{
		Floor * cFloor = ((Floor*)obj);
		Rect * rectA = (Rect *)collision->BoundingBox();
		Rect * rectB = (Rect *)obj->Components()->GetComponent<Collision>()->BoundingBox();

		if (rectA->Bottom() > rectB->Top() && cFloor->Passthrough())
		{
			collision->Parent(obj);
		}

		bool collide = platformScript->PerfectGroundCollision(obj);

		if (collide)
		{
			bDescendingPlatform = false;
			bCanPassThroughPlatform = cFloor->Passthrough();

			if (bFallAnim)
			{
				bGroudingAnim = true;
				bCanFaceAnim = false;
				bFacingAnim = false;
			}

			if (rigidbody->Velocity().Y() == 0)
			{
				bJumpAnim = false;
				bFallAnim = false;
				canJump = true;
			}
		}
	}
}

void Player::OnCollisionStay(Object * obj)
{
	if (!bDescendingPlatform)
	{
		if (obj->Type() == PLATFORM)
		{
			Floor * cFloor = ((Floor*)obj);

			if (platformScript->PerfectGroundCollision(obj))
			{
				if (rigidbody->Velocity().Y() == 0)
				{
					if (bJumpAnim)
					{
						bCanFaceAnim = true;
						bFacingAnim = true;
					}

					canJump = true;

					bJumpAnim = false;
					bFallAnim = false;

					bCanPassThroughPlatform = cFloor->Passthrough();
				}
			}
		}
	}
	else
	{
		if (obj->Type() == PLATFORM)
		{
			Floor * cFloor = ((Floor*)obj);

			if (cFloor->Passthrough())
				collision->Parent(obj);
		}
	}
}

void Player::OnCollisionExit(Object * obj)
{
	if (obj->Type() == PLATFORM)
	{
		bCanPassThroughPlatform = false;
		bSittingAnim = false;
		bSittingPersistAnim = false;
		canJump = false;
	}
}

void Player::ResetVelocity()
{
	rigidbody->ResetVelocity();
	rigidbody->ResetAcceleration();
}

void Player::Update(float gameTime)
{
	//canJump = platformScript->IsGrounded();

	if (Engine::InputManager()->KeyDown(VK_RIGHT))
	{
		if (!bFallAnim && !bJumpAnim)
		{
			animation->Repeat(true);
			animation->Select(WALKING_RIGHT);

			bCanFaceAnim = true;
			bGroudingAnim = false;
			bFacingAnim = true;

			animation->Repeat(true);
			bGettingUpAnim = false;
			bSitting = false;
			bSittingAnim = false;
			bSittingPersistAnim = false;
		}
		else
		{
			if (!bFacingLeft)
			{
				if (bFallAnim)
					animation->Select(FALLING_RIGHT);
				else
					animation->Select(JUMP_RIGHT);
			}
		}

		if (bFacingLeft)
		{
			rigidbody->ResetXAcceleration();
			rigidbody->ResetXVelocity();
		}

		bFacingRight = true;
		bFacingLeft = false;

		Vector2 vec = Vector2(800.0f * Timer::GameTime(), 0);
		rigidbody->AddForce(vec);

		//position->Translate(vec);
	} 
	else if (Engine::InputManager()->KeyDown(VK_LEFT))
	{
		if (!bFallAnim && !bJumpAnim)
		{
			animation->Repeat(true);
			animation->Select(WALKING_LEFT);

			bCanFaceAnim = true;
			bGroudingAnim = false;
			bFacingAnim = true;

			animation->Repeat(true);
			bGettingUpAnim = false;
			bSitting = false;
			bSittingAnim = false;
			bSittingPersistAnim = false;
		}
		else
		{
			if (!bFacingRight)
			{
				if (bFallAnim)
					animation->Select(FALLING_LEFT);
				else
					animation->Select(JUMP_LEFT);
			}
		}

		if (bFacingRight)
		{
			rigidbody->ResetXAcceleration();
			rigidbody->ResetXVelocity();
		}

		bFacingRight = false;
		bFacingLeft = true;

		Vector2 vec = Vector2(-800.0f * Timer::GameTime(), 0);
		rigidbody->AddForce(vec);

		//position->Translate(vec);
	}
	else
	{
		rigidbody->ResetXAcceleration();
		rigidbody->ResetXVelocity();

		if (!bFallAnim && !bJumpAnim)
		{
			if (bFacingRight)
				animation->Select(STILL_RIGHT);
			else if (bFacingLeft)
				animation->Select(STILL_LEFT);
		}
	}

	if (Engine::InputManager()->KeyCtrl('S'))
	{
		int signal = 0;

		if (bFacingRight)
			signal = +1;
		else if (bFacingLeft)
			signal = -1;

		missile = new Missile(0, 28800 * gameTime * signal, 0);
		missile->MoveTo(X() + 10 * signal, Y());

		Engine::CurrentGame()->GetScene()->Add(missile, MOVING);
	}

	if (!platformScript->IsGrounded() || bDescendingPlatform)
	{
		float ySpeed = rigidbody->Velocity().Y();

		if (ySpeed > 0)
		{
			if (!bFallAnim)
			{
				// Caindo
				if (bFacingRight)
					animation->Select(FALLING_RIGHT);
				else if (bFacingLeft)
					animation->Select(FALLING_LEFT);

				bJumpAnim = false;
				bFallAnim = true;

				animation->Repeat(true);
			}
		}
		else if (ySpeed < 0)
		{
			if (!bJumpAnim)
			{
				// Pulando
				if (bFacingRight)
					animation->Select(JUMP_RIGHT);
				else if (bFacingLeft)
					animation->Select(JUMP_LEFT);

				bJumpAnim = true;
				bFallAnim = false;

				animation->Repeat(true);
			}
		}
	}
	
	if (Engine::InputManager()->KeyDown('A') && canJump && !bJumpAnim)
	{
		if (Engine::InputManager()->KeyDown(VK_DOWN) && !bDescendingPlatform && bCanPassThroughPlatform && collision->CollisorTable()->size() == 1)
		{
			collision->TriggerMode(true);
			animation->Repeat(true);

			bPassedThroughPlatform = false;
			passFloor = nullptr;
			bDescendingPlatform = true;
			bSittingAnim = false;
			bSittingPersistAnim = false;
			canJump = false;

			bJumpAnim = true;
			bFallAnim = false;
		}
		else
		{
			// Pulo

			animation->Repeat(true);
			bPassedThroughPlatform = false;
			canJump = false;
			bSittingAnim = false;
			bSittingPersistAnim = false;
			bJumpAnim = false;
			bFallAnim = true;

			rigidbody->ResetYAcceleration();
			rigidbody->AddForce(Vector2(0, -350));
		}
	}

	platformScript->SaveAccelerationVector(rigidbody->Velocity());
}

void Player::Draw()
{
}

const string Player::Name() const
{
	return name;
}

const string Player::CurrentAnimation() const
{
	string animName;

	switch (animation->SelectedId())
	{
		case STILL_RIGHT:
			animName = "STILL_RIGHT";
			break;

		case STILL_LEFT:
			animName = "STILL_LEFT";
			break;

		case SITTING_RIGHT:
			animName = "SITTING_RIGHT";
			break;

		case SITTING_LEFT:
			animName = "SITTING_LEFT";
			break;

		case SAT_RIGHT:
			animName = "SAT_RIGHT";
			break;

		case SAT_LEFT:
			animName = "SAT_LEFT";
			break;

		case SIT_STILL_RIGHT:
			animName = "SIT_STILL_RIGHT";
			break;

		case SIT_STILL_LEFT:
			animName = "SIT_STILL_LEFT";
			break;

		case JUMP_RIGHT:
			animName = "JUMP_RIGHT";
			break;

		case JUMP_LEFT:
			animName = "JUMP_LEFT";
			break;

		case FALLING_RIGHT:
			animName = "FALLING_RIGHT";
			break;

		case FALLING_LEFT:
			animName = "FALLING_LEFT";
			break;

		case GROUNDING_RIGHT:
			animName = "GROUNDING_RIGHT";
			break;

		case GROUNDING_LEFT:
			animName = "GROUNDING_LEFT";
			break;

		case WALKING_RIGHT:
			animName = "WALKING_RIGHT";
			break;

		case WALKING_LEFT:
			animName = "WALKING_LEFT";
			break;

		default:
			animName = "Desconhecido";
	}

	return animName;
}

void Player::SetCheckpoint(float x, float y)
{
	checkPointX = x;
	checkPointY = y;
}

int Player::CheckpointX() const
{
	return checkPointX;
}

int Player::CheckpointY() const
{
	return checkPointY;
}
